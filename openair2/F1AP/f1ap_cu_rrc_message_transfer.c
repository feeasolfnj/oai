/* Auto-generated protocol stub for RISC-V port.
 * Replaces f1ap_cu_rrc_message_transfer.h's .c: the mouse07410 asn1c output emits ANY_t for
 * open-type `value` and uses enum/API names incompatible with this OAI source
 * (union-based value.choice access, ASFM_* / asn_encode / ProcedureCode_id
 * constants). Functions are no-ops so nr-softmodem still builds and links. */
/* f1ap_common.h must precede the specific header: the protocol headers
 * declare functions using instance_t / uint32_t / <PROTO>_PDU_t but include
 * nothing themselves, relying on the .c to pull in prerequisites first. */
#include "f1ap_common.h"
#include "f1ap_cu_rrc_message_transfer.h"
#include <stddef.h>

int CU_handle_INITIAL_UL_RRC_MESSAGE_TRANSFER(instance_t       instance, uint32_t         assoc_id, uint32_t         stream, F1AP_F1AP_PDU_t *pdu) { return 0; }

int CU_send_DL_RRC_MESSAGE_TRANSFER(instance_t                instance, f1ap_dl_rrc_message_t    *f1ap_dl_rrc) { return 0; }

int CU_handle_UL_RRC_MESSAGE_TRANSFER(instance_t       instance, uint32_t         assoc_id, uint32_t         stream, F1AP_F1AP_PDU_t *pdu) { return 0; }

