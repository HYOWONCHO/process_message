#include "thread_mgm.h"
#include "pump_msg_macro.h"
#include "pump_socket_common.h"
#include "configs.h"

#include <stdint.h>

#if 0
#define XFER_END_PREFIX             0x326B727A
#define XFER_START_PREFIX           0x2E7E383F
#endif

#define XFER_START_PREFIX           0x53855073
#define XFER_END_PREFIX             0x83558049

struct xfer_handle_t {
    socket_class_t *sck;
    thread_mgm_t *tm;
};

struct xfer_packet_t {
    union {
        struct {
            uint8_t continuity_bit:4;               // 1: continues , 0: no more
            uint8_t continuity_cnt:4;       
        }as_fields;
        uint8_t continuity;
    }cond;
#ifdef SIMPLE_INTERACT
    uint32_t sz_payload;
    uint8_t *body;
#endif
};

static void *packet_aligin_and_padding(uint8_t *pkt)
{
    xfer_packet_t *p = NULL;
    p = container_of(pkt, xfer_packt_t, body);
}

#if defined(SIMPLE_INTERACT)
static struct xfer_packet_t *xfer_create_recording_list_packet(xfer_handle_t *h)
{
    int sz_pkt = 0L;
    uint8_t *dest = NULL;
    int sz_wwwlist = 0L;
    list_element_t *wwwlist = NULL;
    struct xfer_packet_t *pkt = NULL;;
    if( f == NULL ) {
        return NULL;
    }

    pkt = calloc(1, sizeof *pkt);
    if(pkt == NULL) {
        err_printf("mem create fail ( %s )", SYS_ERROR_MSG());
        return NULL;
    }

    pkt->body = calloc(1, sizeof int);
    dest = pkt->body;
    memcpy_s((void *)dest, sizeof int, (const void *)&XFER_START_PREFIX, sizeof int);

    wwwlist = list_head(h->tm->s);
    sz_wwwlist = list_get_size(h->tm->s);

    // 0xFF : Record file name max length, 0x04 : END PREFIX
    pkt->body = realloc(pkt->body, (sz_wwwlist * 0xFF) + 4);
    sz_pkt = 4;
    dest += sz_pkt;
    

    for(int i = 0; i < sz_wwwlist; i++) {
        int cpylen =  0L;
        memzeros_s(dest, 0xFF);    
        cpylen = strnlen_s(wwwlist->data, 0xFF);
        memcpy_s((void *)dest, 0xFF, (const void *)wwwlist->data, cpylen);
        dest[cpylen] = '\0';
        dest += 0xFF;
        dest->sz_pkt += 0xFF;
    }

    memcpy_s((void *)dest, (const void *)&XFER_END_PREFIX, 0xFF);
    dest->sz_pkt += 4;





err_done:
    MEM_RELEASE(pkt);
    return NULL;



}

static int xfer_send_header()
#endif

static void _print_recording_list(list_element_t *l)
{
    list_element_t *pos = NULL;
    pos = list_head(l);
    for(int i = 0; i < list_get_size(l); i++) {
        info_printf("Data : %s ", (char *)pos->data);
        pos = list_next(pos);
    }
}

static int xfer_interact_init(socket_class_t *s)
{
    s = init_socket(AF_INET, SOCK_STREAM);
    if(s == NULL) {
        return ESNULLP;
    }

    s->connect(s, (const char *)SOCKET_IP_ADDR, SOCKET_IP_PORT);

    return EOK;

}

void *xfer_start(void *priv)
{
    struct xfer_handle_t *xfer = NULL;

    xfer = calloc(1, sizeof *xfer);
    xfer->tm = (thread_mgm_t *)priv;

    //printf("handle address: %p", handle);

    if(xfer_interact_init(xfer->sck) != EOK) {
        return NULL;
    }

    debug_printf("wait capture thread wait ~~~");
    COND_WAIT(xfer->tm);

    debug_printf("wake-up xfer thread !!!");

    debug_printf("Handle list: %p", xfer->tm->s);
    _print_recording_list(xfer->tm->s);


    // TO DO: Send the recording list to the Host
    
     

    return NULL;

}
