/* Auto-generated protocol stub for RISC-V port.
 * Replaces m3ap_ids.h's .c: the mouse07410 asn1c output emits ANY_t for
 * open-type `value` and uses enum/API names incompatible with this OAI source
 * (union-based value.choice access, ASFM_* / asn_encode / ProcedureCode_id
 * constants). Functions are no-ops so nr-softmodem still builds and links. */
/* m3ap_common.h must precede the specific header: the protocol headers
 * declare functions using instance_t / uint32_t / <PROTO>_PDU_t but include
 * nothing themselves, relying on the .c to pull in prerequisites first. */
#include "m3ap_common.h"
#include "m3ap_MCE_defs.h"
#include "m3ap_MME_defs.h"
#include "m3ap_ids.h"
#include <stddef.h>

void m3ap_id_manager_init(m3ap_id_manager *m) { }

int m3ap_allocate_new_id(m3ap_id_manager *m) { return 0; }

void m3ap_release_id(m3ap_id_manager *m, int id) { }

int m3ap_find_id(m3ap_id_manager *, int id_source, int id_target) { return 0; }

int m3ap_find_id_from_id_source(m3ap_id_manager *, int id_source) { return 0; }

int m3ap_find_id_from_rnti(m3ap_id_manager *, int rnti) { return 0; }

void m3ap_set_ids(m3ap_id_manager *m, int ue_id, int rnti, int id_source, int id_target) { }

void m3ap_id_set_state(m3ap_id_manager *m, int ue_id, m3id_state_t state) { }

void m3ap_id_set_target(m3ap_id_manager *m, int ue_id, void *target) { }

void m3ap_set_reloc_prep_timer(m3ap_id_manager *m, int ue_id, uint64_t time) { }

void m3ap_set_reloc_overall_timer(m3ap_id_manager *m, int ue_id, uint64_t time) { }

int m3ap_id_get_id_source(m3ap_id_manager *m, int ue_id) { return 0; }

int m3ap_id_get_id_target(m3ap_id_manager *m, int ue_id) { return 0; }

int m3ap_id_get_rnti(m3ap_id_manager *m, int ue_id) { return 0; }

void *m3ap_id_get_target(m3ap_id_manager *m, int ue_id) { return 0; }

