/* Auto-generated protocol stub for RISC-V port.
 * Replaces f1ap_cu_ue_context_management.h's .c: the mouse07410 asn1c output emits ANY_t for
 * open-type `value` and uses enum/API names incompatible with this OAI source
 * (union-based value.choice access, ASFM_* / asn_encode / ProcedureCode_id
 * constants). Functions are no-ops so nr-softmodem still builds and links. */
/* f1ap_common.h must precede the specific header: the protocol headers
 * declare functions using instance_t / uint32_t / <PROTO>_PDU_t but include
 * nothing themselves, relying on the .c to pull in prerequisites first. */
#include "f1ap_common.h"
#include "f1ap_cu_ue_context_management.h"
#include <stddef.h>

int CU_send_UE_CONTEXT_SETUP_REQUEST(instance_t instance, f1ap_ue_context_setup_t *f1ap_ue_context_setup_req) { return 0; }

int CU_handle_UE_CONTEXT_SETUP_RESPONSE(instance_t       instance, uint32_t         assoc_id, uint32_t         stream, F1AP_F1AP_PDU_t *pdu) { return 0; }

int CU_handle_UE_CONTEXT_SETUP_FAILURE(instance_t       instance, uint32_t         assoc_id, uint32_t         stream, F1AP_F1AP_PDU_t *pdu) { return 0; }

int CU_handle_UE_CONTEXT_RELEASE_REQUEST(instance_t       instance, uint32_t         assoc_id, uint32_t         stream, F1AP_F1AP_PDU_t *pdu) { return 0; }

int CU_send_UE_CONTEXT_RELEASE_COMMAND(instance_t instance, f1ap_ue_context_release_cmd_t *cmd) { return 0; }

int CU_handle_UE_CONTEXT_RELEASE_COMPLETE(instance_t       instance, uint32_t         assoc_id, uint32_t         stream, F1AP_F1AP_PDU_t *pdu) { return 0; }

int CU_send_UE_CONTEXT_MODIFICATION_REQUEST(instance_t instance, f1ap_ue_context_modif_req_t *f1ap_ue_context_modification_req) { return 0; }

int CU_handle_UE_CONTEXT_MODIFICATION_RESPONSE(instance_t       instance, uint32_t         assoc_id, uint32_t         stream, F1AP_F1AP_PDU_t *pdu) { return 0; }

int CU_handle_UE_CONTEXT_MODIFICATION_FAILURE(instance_t       instance, uint32_t         assoc_id, uint32_t         stream, F1AP_F1AP_PDU_t *pdu) { return 0; }

int CU_handle_UE_CONTEXT_MODIFICATION_REQUIRED(instance_t       instance, uint32_t         assoc_id, uint32_t         stream, F1AP_F1AP_PDU_t *pdu) { return 0; }

int CU_send_UE_CONTEXT_MODIFICATION_CONFIRM(instance_t instance, F1AP_UEContextModificationConfirm_t UEContextModificationConfirm_t) { return 0; }

