#include "if_packet.h"
#include "pump_msg_macro.h"


static int is_include_optional_valid(if_pkt_main_t *p)
{
    uint8_t is_validopt = 0;

    is_validopt = ((p->len >> 12) & 0b1000) >> 0x03;
    return is_validopt;
}


int if_tag_processing(tlv_t *in, void *out)
{
    int ret = 0L;

    switch(in->tag) {
        case REQ_H2T_SYSTEM_STATUS:
            //TODO: Check the state for System 
            break;
        case REQ_H2T_CAM_SENSOR_INFO:
            //TODO: Read the Sensor Information over RTSP 
            break;
        case REQ_H2T_APP_UPDATE:
            //TODO: Application Update for Camera Solution
            break;
        case REQ_H2T_OP_POLICY:
            //TODO: Set the policy for Application working 
            break;
        case REQ_H2T_LATEST_DETECT:
            //TODO: Latested detecting information processing 
            break;
        default:
            ret = RET_UNKNOWN_TYPE;
            break;

    }

done:
    return ret;

}


int if_incoming_packet_dispatch(if_pkt_main_t *p) {
    int ret = 0;
    if_msgbody_t *msg; 
    int is_validopt = 0L;


    RETURN_VAL_IF_FAIL(p != NULL, RET_INV_PARAM);
    RETURN_VAL_IF_FAIL((p->aid != PACKET_AID), RET_INV_AID);
    RETURN_VAL_IF_FAIL((p->len > 0), RET_FAIL);


    // check whether Message HASH value has.
    is_validopt = is_include_optional_valid(p);

    if(is_validopt) {
        // TODO: Message verification using HASH-256
    }

    msg = (if_msgbody_t *)&p->body[0];

    RETURN_VAL_IF_FAIL(msg != NULL, RET_INV_OBJ);

    // TODO : CLASS, TYPE Check 


    if(msg->msghd->param > 0) {
        uint8_t p1 = msg->msghd->param & 0xFF;
        uint8_t p2 = (msg->msghd->param >> 8) & 0xFF;
    }

    if(if_tag_processing((tlv_t *)&msg->in[0]) < 0) {
        ret = -1;
        goto done;
    }

done:

    return ret;


}


