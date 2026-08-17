/* Auto-generated protocol stub for RISC-V port.
 * Replaces m3ap_MCE_management_procedures.h's .c: the mouse07410 asn1c output emits ANY_t for
 * open-type `value` and uses enum/API names incompatible with this OAI source
 * (union-based value.choice access, ASFM_* / asn_encode / ProcedureCode_id
 * constants). Functions are no-ops so nr-softmodem still builds and links. */
/* m3ap_common.h must precede the specific header: the protocol headers
 * declare functions using instance_t / uint32_t / <PROTO>_PDU_t but include
 * nothing themselves, relying on the .c to pull in prerequisites first. */
#include "m3ap_common.h"
#include "m3ap_MCE_defs.h"
#include "m3ap_MME_defs.h"
#include "m3ap_MCE_management_procedures.h"
#include <stddef.h>

void m3ap_MCE_prepare_internal_data(void) { }

void dump_trees_m3(void) { }

void m3ap_MCE_insert_new_instance(m3ap_MCE_instance_t *new_instance_p) { }

m3ap_MCE_instance_t *m3ap_MCE_get_instance(uint8_t mod_id) { return 0; }

uint16_t m3ap_MCE_fetch_add_global_cnx_id(void) { return 0; }

