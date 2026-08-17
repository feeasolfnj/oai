/* Auto-generated protocol stub for RISC-V port.
 * Replaces m3ap_timers.h's .c: the mouse07410 asn1c output emits ANY_t for
 * open-type `value` and uses enum/API names incompatible with this OAI source
 * (union-based value.choice access, ASFM_* / asn_encode / ProcedureCode_id
 * constants). Functions are no-ops so nr-softmodem still builds and links. */
/* m3ap_common.h must precede the specific header: the protocol headers
 * declare functions using instance_t / uint32_t / <PROTO>_PDU_t but include
 * nothing themselves, relying on the .c to pull in prerequisites first. */
#include "m3ap_common.h"
#include "m3ap_MCE_defs.h"
#include "m3ap_MME_defs.h"
#include "m3ap_timers.h"
#include <stddef.h>

void m3ap_timers_init(m3ap_timers_t *t, int t_reloc_prep, int tm3_reloc_overall) { }

void m3ap_check_timers(instance_t instance) { }

uint64_t m3ap_timer_get_tti(m3ap_timers_t *t) { return 0; }

