#include "cilix_task.h"

cilix_task_q_t* cilix_task_q_malloc(void* udata, void* w, void* sem) {
    cilix_task_q_t* q = (cilix_task_q_t*)malloc(sizeof(cilix_task_q_t));

    q->u_data = udata;

    q->w = w;
    q->sem = sem;

    QUEUE_INIT(&q->node);

    return q;
}

void cilix_task_q_free(cilix_task_q_t* q) {
    if (q == 0) return;

    free(q);
}

void cilix_q_done(void* p) {
    if (p != 0) {
        uv_sem_t* sem = (uv_sem_t*)p;
        uv_sem_post(sem);
    }
}

cilix_task_t* cilix_task_malloc(
    void* udata, 
    cilix_task_init_cb uicb, cilix_task_fini_cb ufcb,
    cilix_task_q_get_cb uqgcb, cilix_task_q_clean_cb uqccb) {
    cilix_task_t *task = (cilix_task_t*)malloc(sizeof(cilix_task_t));

    task->u_data = udata;
    task->u_init_cb = uicb;
    task->u_fini_cb = ufcb;
    task->u_q_get_cb = uqgcb;
    task->u_q_clean_cb = uqccb;

    uv_mutex_init(&task->mtx);
    uv_cond_init(&task->cond);

    task->run = 0;
    task->empty_exit = 0;

    QUEUE_INIT(&task->qqueue);

    return task;
}

void cilix_task_free(cilix_task_t* task) {
    if (task == 0) return;

    while (!QUEUE_EMPTY(&task->qqueue)) {
        QUEUE* head = QUEUE_HEAD(&task->qqueue);
        QUEUE_REMOVE(head);

        cilix_task_q_t* q = QUEUE_DATA(head, cilix_task_q_t, node);
        if (q->u_data != 0 && task->u_q_clean_cb != 0) {
            task->u_q_clean_cb(task, q->u_data);
        }
        cilix_task_q_free(q);
    }

    uv_mutex_destroy(&task->mtx);
    uv_cond_destroy(&task->cond);

    free(task);
}

void cilix_task_putq(cilix_task_t* task, void* udata, int wait, void* w) {
    uv_sem_t f_sem;

    uv_sem_t* sem = 0;
    if (wait) {
        sem = &f_sem;
        uv_sem_init(sem, 0);
    }

    cilix_task_q_t* q = cilix_task_q_malloc(udata, w, sem);

    uv_mutex_lock(&task->mtx);
    QUEUE_INSERT_TAIL(&task->qqueue, &q->node);
    uv_mutex_unlock(&task->mtx);

    uv_cond_signal(&task->cond);

    if (wait) {
        uv_sem_wait(sem);
        uv_sem_destroy(sem);
    }
}

int cilix_task_getq(cilix_task_t* task, void** udata, void** w, void** sem) {
    int iret = CILIX_TASK_GETQ_RET_OK;
    uv_mutex_lock(&task->mtx);

    while (task->run == 1 && QUEUE_EMPTY(&task->qqueue) && task->empty_exit == 0) {
        uv_cond_wait(&task->cond, &task->mtx);
    }

    do {
        if (task->run != 1) {
            iret = CILIX_TASK_GETQ_RET_STOP;
            break;
        }
        if (!QUEUE_EMPTY(&task->qqueue)) {
            QUEUE *head = QUEUE_HEAD(&task->qqueue);
            QUEUE_REMOVE(head);

            cilix_task_q_t *q = QUEUE_DATA(head, cilix_task_q_t, node);
            *udata = q->u_data;
            *w = q->w;
            *sem = q->sem;
            cilix_task_q_free(q);

            iret = CILIX_TASK_GETQ_RET_OK;
            break;
        }
        if (task->empty_exit != 0) {
            iret = CILIX_TASK_GETQ_RET_EMPTY_EXIT;
            break;
        }
    } while(0);

    uv_mutex_unlock(&task->mtx);
    return iret;
}

void cilix_task_thr(void* arg) {
    cilix_task_t* task = (cilix_task_t*)arg;

    if (task->u_init_cb != 0) {
        task->u_init_cb(task);
    }

    for (int iret=CILIX_TASK_GETQ_RET_OK; iret==CILIX_TASK_GETQ_RET_OK;) {
        void* udata = 0;
        void* w = 0;
        void* sem = 0;
        iret = cilix_task_getq(task, &udata, &w, &sem);

        if (udata != 0 && task->u_q_get_cb != 0) {
            task->u_q_get_cb(task, udata, w, cilix_q_done, sem);
        }
    }

    if (task->u_fini_cb != 0) {
        task->u_fini_cb(task);
    }
}

void cilix_task_start(cilix_task_t* task) {
    task->run = 1;
    uv_thread_create(&task->t_id, cilix_task_thr, task);
}

void cilix_task_stop(cilix_task_t* task) {
    task->run = 0;
    uv_cond_broadcast(&task->cond);
}

void cilix_task_empty_exit(cilix_task_t* task) {
    task->empty_exit = 1;
    uv_cond_broadcast(&task->cond);
}

void cilix_task_wait(cilix_task_t* task) {
    uv_thread_join(&task->t_id);
}