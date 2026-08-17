/* Auto-generated protocol stub for RISC-V port.
 * Replaces e1ap.h's .c: the mouse07410 asn1c output emits ANY_t for
 * open-type `value` and uses enum/API names incompatible with this OAI source
 * (union-based value.choice access, ASFM_* / asn_encode / ProcedureCode_id
 * constants). Functions are no-ops so nr-softmodem still builds and links. */
/* e1ap_common.h must precede the specific header: the protocol headers
 * declare functions using instance_t / uint32_t / <PROTO>_PDU_t but include
 * nothing themselves, relying on the .c to pull in prerequisites first. */
#include "e1ap_common.h"
#include "e1ap.h"
#include <stddef.h>

int e1apCUCP_handle_SETUP_REQUEST(e1ap_upcp_inst_t *inst, const E1AP_E1AP_PDU_t *pdu) { return 0; }

int e1apCUUP_handle_SETUP_RESPONSE(e1ap_upcp_inst_t *inst, const E1AP_E1AP_PDU_t *pdu) { return 0; }

int e1apCUUP_handle_SETUP_FAILURE(e1ap_upcp_inst_t *inst, const E1AP_E1AP_PDU_t *pdu) { return 0; }

int e1apCUUP_handle_BEARER_CONTEXT_SETUP_REQUEST(e1ap_upcp_inst_t *inst, const E1AP_E1AP_PDU_t *pdu) { return 0; }

int e1apCUCP_handle_BEARER_CONTEXT_SETUP_RESPONSE(e1ap_upcp_inst_t *inst, const E1AP_E1AP_PDU_t *pdu) { return 0; }

int e1apCUCP_handle_BEARER_CONTEXT_SETUP_FAILURE(e1ap_upcp_inst_t *inst, const E1AP_E1AP_PDU_t *pdu) { return 0; }

int e1apCUUP_handle_BEARER_CONTEXT_MODIFICATION_REQUEST(e1ap_upcp_inst_t *inst, const E1AP_E1AP_PDU_t *pdu) { return 0; }

void e1apCUUP_send_BEARER_CONTEXT_SETUP_RESPONSE(e1ap_upcp_inst_t *inst, e1ap_bearer_setup_resp_t *const resp) { }

int e1apCUUP_handle_BEARER_CONTEXT_RELEASE_COMMAND(e1ap_upcp_inst_t *inst, const E1AP_E1AP_PDU_t *pdu) { return 0; }

int e1apCUCP_handle_BEARER_CONTEXT_RELEASE_COMPLETE(e1ap_upcp_inst_t *inst, const E1AP_E1AP_PDU_t *pdu) { return 0; }

int e1apCUUP_send_BEARER_CONTEXT_RELEASE_COMPLETE(e1ap_upcp_inst_t *inst, e1ap_bearer_release_cmd_t *const cmd) { return 0; }

void *E1AP_CUUP_task(void *arg) { return 0; }

void *E1AP_CUCP_task(void *arg) { return 0; }

