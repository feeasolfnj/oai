/* Auto-generated protocol stub for RISC-V port.
 * Replaces m3ap_itti_messaging.h's .c: the mouse07410 asn1c output emits ANY_t for
 * open-type `value` and uses enum/API names incompatible with this OAI source
 * (union-based value.choice access, ASFM_* / asn_encode / ProcedureCode_id
 * constants). Functions are no-ops so nr-softmodem still builds and links. */
/* m3ap_common.h must precede the specific header: the protocol headers
 * declare functions using instance_t / uint32_t / <PROTO>_PDU_t but include
 * nothing themselves, relying on the .c to pull in prerequisites first. */
#include "m3ap_common.h"
#include "m3ap_MCE_defs.h"
#include "m3ap_MME_defs.h"
#include "m3ap_itti_messaging.h"
#include <stddef.h>

void m3ap_MCE_itti_send_sctp_data_req(instance_t instance, int32_t assoc_id, uint8_t *buffer, uint32_t buffer_length, uint16_t stream) { }

void m3ap_MCE_itti_send_sctp_close_association(instance_t instance, int32_t assoc_id) { }

void m3ap_MME_itti_send_sctp_data_req(instance_t instance, int32_t assoc_id, uint8_t *buffer, uint32_t buffer_length, uint16_t stream) { }

void m3ap_MME_itti_send_sctp_close_association(instance_t instance, int32_t assoc_id) { }

