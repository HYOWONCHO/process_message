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


#define XFER_PACKET_BLKSZ           0x40


#define XFER_PADDING_MOD(s)         (s % XFER_PACKET_BLKSZ)

#define XFER_PADDING_CAL(s)         (XFER_PACKET_BLKSZ - XFER_PADDING_MOD(s))

struct xfer_packet_t {
#if 0
    union {
        struct {
            uint8_t continuity_bit:4;               // 1: continues , 0: no more
            uint8_t continuity_cnt:4;       
        }as_fields;
        uint8_t continuity;
    }cond;
#endif
    uint32_t sz_payload;
    uint8_t *body;
};


struct xfer_handle_t {
    struct xfer_packet_t *pkt;
    socket_class_t *sck;
    thread_mgm_t *tm;
    
};

static void *packet_aligin_and_padding(uint8_t *pkt)
{
    struct xfer_packet_t *p = NULL;
    p = container_of(pkt, struct xfer_packet_t, body);

    return NULL;
}

#if defined(SIMPLE_INTERACT)
//static struct xfer_packet_t *xfer_create_recording_list_packet(xfer_handle_t *h)
struct xfer_packet_t *xfer_create_recording_list_packet(struct xfer_handle_t *h)
{
    uint32_t start_prefix = XFER_START_PREFIX;
    uint32_t end_prefix = XFER_END_PREFIX;
    int sz_pkt = 0L;
    uint8_t *dest = NULL;
    int sz_wwwlist = 0L;
    list_element_t *wwwlist = NULL;
    struct xfer_packet_t *pkt = NULL;;

    if( h == NULL ) {
        err_printf("Invalid parameter");
        return NULL;
    }

    pkt = calloc(1, sizeof *pkt);
    if(pkt == NULL) {
        err_printf("mem create fail ( %s )", SYS_ERROR_MSG());
        return NULL;
    }


    pkt->body = calloc(1, sizeof(int));
    dest = pkt->body;
    memcpy_s((void *)dest, sizeof(int), (const void *)&start_prefix, sizeof(int));

    wwwlist = list_head(h->tm->s);
    sz_wwwlist = list_get_size(h->tm->s);

    sz_pkt = 4;
    __BUF_HEX_PRINT(pkt->body, "start packet", sz_pkt);
    // 0xFF : Record file name max length, 0x04 : END PREFIX
    //pkt->body = realloc(pkt->body, (sz_wwwlist * 0xFF) +  sz_pkt);
    dest += sz_pkt;
    
#if 0
    for(int i = 0; i < sz_wwwlist; i++) {
        int cpylen =  0L;
        memzero_s(dest, 0xFF);    
        cpylen = strnlen_s(wwwlist->data, 0xFF);
        memcpy_s((void *)dest, 0xFF, (const void *)wwwlist->data, cpylen);
        wwwlist = list_next(wwwlist);
        dest[cpylen] = '\0';
        dest += 0xFF;
        sz_pkt += 0xFF;
    }
#endif

    for(int i = 0; i < sz_wwwlist; i++) {
        int cpylen =  0L;
        cpylen = strnlen_s(wwwlist->data, 0xFF);
        sz_pkt += cpylen + 1;
        pkt->body = realloc(pkt->body, sz_pkt);
        memzero_s(dest, cpylen);    
        memcpy_s((void *)dest, cpylen, (const void *)wwwlist->data, cpylen);
        wwwlist = list_next(wwwlist);
        dest[cpylen] = '\0';
        dest += cpylen+1;
        //sz_pkt += cpylen+1; 
    }

    __BUF_HEX_PRINT(pkt->body, "list packet", sz_pkt);


    sz_pkt += 4;
    pkt->body = realloc(pkt->body, sz_pkt);
    memcpy_s((void *)dest, 4, (const void *)&end_prefix, 4);
    dest += 4;

    if( (XFER_PADDING_MOD(sz_pkt) != 0) &&  (sz_pkt != 0)) {
        int remind_padding = XFER_PADDING_CAL(sz_pkt);
        pkt->body = realloc(pkt->body, remind_padding + sz_pkt);
        memzero_s(dest, remind_padding);
        sz_pkt += remind_padding;
    }

    h->pkt = pkt;
    h->pkt->sz_payload = sz_pkt;


err_done:
    //MEM_RELEASE(pkt);
    return NULL;
}

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

static int xfer_interact_init(socket_class_t **s)
{
    socket_class_t *_s;
    _s = init_socket(AF_INET, SOCK_STREAM);
    if(_s == NULL) {
        return ESNULLP;
    }

    _s->connect(_s, (const char *)SOCKET_IP_ADDR, SOCKET_IP_PORT);

    *s = _s;

    return EOK;

}

void *xfer_start(void *priv)
{
    struct xfer_handle_t *xfer = NULL;

    xfer = calloc(1, sizeof *xfer);
    xfer->tm = (thread_mgm_t *)priv;

    //printf("handle address: %p", handle);

    if(xfer_interact_init(&xfer->sck) != EOK) {
       return NULL;
    }

    debug_printf("wait capture thread wait ~~~");
    //COND_WAIT(xfer->tm);

    debug_printf("wake-up xfer thread !!!");

    debug_printf("Handle list: %p", xfer->tm->s);
    _print_recording_list(xfer->tm->s);


    // TO DO: Send the recording list to the Host
    xfer_create_recording_list_packet(xfer);
    __BUF_HEX_PRINT(xfer->pkt->body, "sending packet", xfer->pkt->sz_payload);
    

    debug_printf();
    xfer->sck->send(xfer->sck, xfer->pkt->body, 128);
    debug_printf();
     
    MEM_RELEASE(xfer->pkt);
    return NULL;

}
