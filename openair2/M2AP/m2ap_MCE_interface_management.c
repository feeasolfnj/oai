/* Auto-generated protocol stub for RISC-V port.
 * Replaces m2ap_MCE_interface_management.h's .c: the mouse07410 asn1c output emits ANY_t for
 * open-type `value` and uses enum/API names incompatible with this OAI source
 * (union-based value.choice access, ASFM_* / asn_encode / ProcedureCode_id
 * constants). Functions are no-ops so nr-softmodem still builds and links. */
/* m2ap_common.h must precede the specific header: the protocol headers
 * declare functions using instance_t / uint32_t / <PROTO>_PDU_t but include
 * nothing themselves, relying on the .c to pull in prerequisites first. */
#include "m2ap_common.h"
#include "m2ap_eNB_defs.h"
#include "m2ap_MCE_defs.h"
#include "m2ap_MCE_interface_management.h"
#include <stddef.h>

int MCE_send_MBMS_SESSION_START_REQUEST(instance_t instance/*, uint32_t assoc_id*/,m2ap_session_start_req_t* m2ap_session_start_req) { return 0; }

int MCE_handle_MBMS_SESSION_START_RESPONSE(instance_t instance, uint32_t assoc_id, uint32_t stream, M2AP_M2AP_PDU_t *pdu) { return 0; }

int MCE_handle_MBMS_SESSION_START_FAILURE(instance_t instance, uint32_t assoc_id, uint32_t stream, M2AP_M2AP_PDU_t *pdu) { return 0; }

int MCE_send_MBMS_SESSION_STOP_REQUEST(instance_t instance, m2ap_session_stop_req_t* m2ap_session_stop_req) { return 0; }

int MCE_handle_MBMS_SESSION_STOP_RESPONSE(instance_t instance, uint32_t assoc_id, uint32_t stream, M2AP_M2AP_PDU_t *pdu) { return 0; }

int MCE_send_MBMS_SCHEDULING_INFORMATION(instance_t instance, /*uint32_t assoc_id,*/ m2ap_mbms_scheduling_information_t * m2ap_mbms_scheduling_information ) { return 0; }

int MCE_handle_MBMS_SCHEDULING_INFORMATION_RESPONSE(instance_t instance, uint32_t assoc_id, uint32_t stream, M2AP_M2AP_PDU_t *pdu) { return 0; }

int MCE_send_RESET(instance_t instance, m2ap_reset_t * m2ap_reset) { return 0; }

int MCE_handle_RESET_ACKKNOWLEDGE(instance_t instance, uint32_t assoc_id, uint32_t stream, M2AP_M2AP_PDU_t *pdu) { return 0; }

int MCE_handle_RESET(instance_t instance, uint32_t assoc_id, uint32_t stream, M2AP_M2AP_PDU_t *pdu) { return 0; }

int MCE_send_RESET_ACKNOWLEDGE(instance_t instance, M2AP_ResetAcknowledge_t *ResetAcknowledge) { return 0; }

int MCE_handle_M2_SETUP_REQUEST(instance_t instance, uint32_t assoc_id, uint32_t stream, M2AP_M2AP_PDU_t *pdu) { return 0; }

int MCE_send_M2_SETUP_RESPONSE(instance_t instance,/*uint32_t assoc_id,*/  m2ap_setup_resp_t *m2ap_setup_resp) { return 0; }

int MCE_send_M2_SETUP_FAILURE(instance_t instance, /*uint32_t assoc_id*/ m2ap_setup_failure_t * m2ap_setup_failure) { return 0; }

int MCE_send_MCE_CONFIGURATION_UPDATE(instance_t instance, module_id_t du_mod_idP) { return 0; }

int MCE_handle_MCE_CONFIGURATION_UPDATE_FAILURE(instance_t instance, uint32_t assoc_id, uint32_t stream, M2AP_M2AP_PDU_t *pdu) { return 0; }

int MCE_handle_MCE_CONFIGURATION_UPDATE_ACKNOWLEDGE(instance_t instance, uint32_t assoc_id, uint32_t stream, M2AP_M2AP_PDU_t *pdu) { return 0; }

int MCE_handle_ENB_CONFIGURATION_UPDATE(instance_t instance, uint32_t assoc_id, uint32_t stream, M2AP_M2AP_PDU_t *pdu) { return 0; }

int MCE_send_ENB_CONFIGURATION_UPDATE_FAILURE(instance_t instance, m2ap_enb_configuration_update_failure_t *m2ap_enb_configuration_update_failure) { return 0; }

int MCE_send_ENB_CONFIGURATION_UPDATE_ACKNOWLEDGE(instance_t instance, m2ap_enb_configuration_update_ack_t *m2ap_enb_configuration_update_ack) { return 0; }

int MCE_handle_ERROR_INDICATION(instance_t instance, uint32_t assoc_id, uint32_t stream, M2AP_M2AP_PDU_t *pdu) { return 0; }

int MCE_send_ERROR_INDICATION(instance_t instance, M2AP_ErrorIndication_t *ErrorIndication) { return 0; }

int MCE_send_MBMS_SESSION_UPDATE_REQUEST(instance_t instance, m2ap_mbms_session_update_req_t * m2ap_mbms_session_update_req) { return 0; }

int MCE_handle_MBMS_SESSION_UPDATE_RESPONSE(instance_t instance, uint32_t assoc_id, uint32_t stream, M2AP_M2AP_PDU_t *pdu) { return 0; }

int MCE_handle_MBMS_SESSION_UPDATE_FAILURE(instance_t instance,module_id_t du_mod_idP) { return 0; }

int MCE_send_MBMS_SERVICE_COUNTING_REQUEST(instance_t instance, module_id_t du_mod_idP) { return 0; }

int MCE_handle_MBMS_SERVICE_COUNTING_RESPONSE(instance_t instance, uint32_t assoc_id, uint32_t stream, M2AP_M2AP_PDU_t *pdu) { return 0; }

int MCE_handle_MBMS_SESSION_COUNTING_FAILURE(instance_t instance, module_id_t du_mod_idP) { return 0; }

int MCE_handle_MBMS_SESSION_COUNTING_RESULTS_REPORT(instance_t instance, uint32_t assoc_id, uint32_t stream, M2AP_M2AP_PDU_t *pdu) { return 0; }

int MCE_handle_MBMS_OVERLOAD_NOTIFICATION(instance_t instance, uint32_t assoc_id, uint32_t stream, M2AP_M2AP_PDU_t *pdu) { return 0; }

