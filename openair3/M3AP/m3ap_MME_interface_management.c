/* Auto-generated protocol stub for RISC-V port.
 * Replaces m3ap_MME_interface_management.h's .c: the mouse07410 asn1c output emits ANY_t for
 * open-type `value` and uses enum/API names incompatible with this OAI source
 * (union-based value.choice access, ASFM_* / asn_encode / ProcedureCode_id
 * constants). Functions are no-ops so nr-softmodem still builds and links. */
/* m3ap_common.h must precede the specific header: the protocol headers
 * declare functions using instance_t / uint32_t / <PROTO>_PDU_t but include
 * nothing themselves, relying on the .c to pull in prerequisites first. */
#include "m3ap_common.h"
#include "m3ap_MCE_defs.h"
#include "m3ap_MME_defs.h"
#include "m3ap_MME_interface_management.h"
#include <stddef.h>

int MME_send_MBMS_SESSION_START_REQUEST(instance_t instance/*, uint32_t assoc_id*/,m3ap_session_start_req_t* m3ap_session_start_req) { return 0; }

int MME_handle_MBMS_SESSION_START_RESPONSE(instance_t instance, uint32_t assoc_id, uint32_t stream, M3AP_M3AP_PDU_t *pdu) { return 0; }

int MME_handle_MBMS_SESSION_START_FAILURE(instance_t instance, uint32_t assoc_id, uint32_t stream, M3AP_M3AP_PDU_t *pdu) { return 0; }

int MME_send_MBMS_SESSION_STOP_REQUEST(instance_t instance, m3ap_session_stop_req_t* m3ap_session_stop_req) { return 0; }

int MME_handle_MBMS_SESSION_STOP_RESPONSE(instance_t instance, uint32_t assoc_id, uint32_t stream, M3AP_M3AP_PDU_t *pdu) { return 0; }

int MME_handle_MBMS_SESSION_UPDATE_RESPONSE(instance_t instance, uint32_t assoc_id, uint32_t stream, M3AP_M3AP_PDU_t *pdu) { return 0; }

int MME_handle_MBMS_SESSION_UPDATE_FAILURE(instance_t instance, uint32_t assoc_id, uint32_t stream, M3AP_M3AP_PDU_t *pdu) { return 0; }

int MME_send_RESET(instance_t instance, m3ap_reset_t * m3ap_reset) { return 0; }

int MME_handle_RESET_ACKKNOWLEDGE(instance_t instance, uint32_t assoc_id, uint32_t stream, M3AP_M3AP_PDU_t *pdu) { return 0; }

int MME_handle_RESET(instance_t instance, uint32_t assoc_id, uint32_t stream, M3AP_M3AP_PDU_t *pdu) { return 0; }

int MME_send_RESET_ACKNOWLEDGE(instance_t instance, void *ResetAcknowledge) { return 0; }

int MME_handle_M3_SETUP_REQUEST(instance_t instance, uint32_t assoc_id, uint32_t stream, M3AP_M3AP_PDU_t *pdu) { return 0; }

int MME_send_M3_SETUP_RESPONSE(instance_t instance,/*uint32_t assoc_id,*/  m3ap_setup_resp_t *m3ap_setup_resp) { return 0; }

int MME_send_M3_SETUP_FAILURE(instance_t instance, /*uint32_t assoc_id*/ m3ap_setup_failure_t * m3ap_setup_failure) { return 0; }

