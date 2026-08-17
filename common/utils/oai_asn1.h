/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this file
 * except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.openairinterface.org/?page_id=698
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */

#ifndef ASN1_CONVERSIONS_H_
#define ASN1_CONVERSIONS_H_

#include "BIT_STRING.h"
#include "assertions.h"

//-----------------------begin func -------------------

/*! \fn uint8_t BIT_STRING_to_uint8(BIT_STRING_t *)
 *\brief  This function extract at most a 8 bits value from a BIT_STRING_t object, the exact bits number depend on the BIT_STRING_t contents.
 *\param[in] pointer to the BIT_STRING_t object.
 *\return the extracted value.
 */
static inline uint8_t BIT_STRING_to_uint8(BIT_STRING_t *asn) {
  DevCheck ((asn->size == 1), asn->size, 0, 0);

  return asn->buf[0] >> asn->bits_unused;
}

/*! \fn uint16_t BIT_STRING_to_uint16(BIT_STRING_t *)
 *\brief  This function extract at most a 16 bits value from a BIT_STRING_t object, the exact bits number depend on the BIT_STRING_t contents.
 *\param[in] pointer to the BIT_STRING_t object.
 *\return the extracted value.
 */
static inline uint16_t BIT_STRING_to_uint16(BIT_STRING_t *asn) {
  uint16_t result = 0;
  int index = 0;

  DevCheck ((asn->size > 0) && (asn->size <= 2), asn->size, 0, 0);

  switch (asn->size) {
    case 2:
      result |= asn->buf[index++] << (8 - asn->bits_unused);

    case 1:
      result |= asn->buf[index] >> asn->bits_unused;
      break;

    default:
      break;
  }

  return result;
}

/*! \fn uint32_t BIT_STRING_to_uint32(BIT_STRING_t *)
 *\brief  This function extract at most a 32 bits value from a BIT_STRING_t object, the exact bits number depend on the BIT_STRING_t contents.
 *\param[in] pointer to the BIT_STRING_t object.
 *\return the extracted value.
 */
static inline uint32_t BIT_STRING_to_uint32(BIT_STRING_t *asn) {
  uint32_t result = 0;
  size_t index;

  DevCheck ((asn->size > 0) && (asn->size <= 4), asn->size, 0, 0);

  int shift = ((asn->size - 1) * 8) - asn->bits_unused;
  for (index = 0; index < (asn->size - 1); index++) {
    result |= asn->buf[index] << shift;
    shift -= 8;
  }

  result |= asn->buf[index] >> asn->bits_unused;

  return result;
}

/*! \fn uint64_t BIT_STRING_to_uint64(BIT_STRING_t *)
 *\brief  This function extract at most a 64 bits value from a BIT_STRING_t object, the exact bits number depend on the BIT_STRING_t contents.
 *\param[in] pointer to the BIT_STRING_t object.
 *\return the extracted value.
 */
static inline uint64_t BIT_STRING_to_uint64(BIT_STRING_t *asn) {
  uint64_t result = 0;
  size_t index;
  int shift;

  DevCheck ((asn->size > 0) && (asn->size <= 8), asn->size, 0, 0);

  shift = ((asn->size - 1) * 8) - asn->bits_unused;
  for (index = 0; index < (asn->size - 1); index++) {
    result |= ((uint64_t)asn->buf[index]) << shift;
    shift -= 8;
  }

  result |= ((uint64_t)asn->buf[index]) >> asn->bits_unused;

  return result;
}

#define asn1cSeqAdd(VaR, PtR) if (ASN_SEQUENCE_ADD(VaR,PtR)!=0) AssertFatal(false, "ASN.1 encoding error " #VaR "\n")
#define asn1cCallocOne(VaR, VaLue) \
  VaR = calloc(1,sizeof(*VaR)); *VaR=VaLue
#define asn1cCalloc(VaR, lOcPtr) \
  typeof(VaR) lOcPtr = VaR = calloc(1,sizeof(*VaR))
#define asn1cSequenceAdd(VaR, TyPe, lOcPtr) \
  TyPe *lOcPtr= calloc(1,sizeof(TyPe)); \
  asn1cSeqAdd(&VaR,lOcPtr)

/*
 * asn1c free-method constants.
 * The generated asn1c in this toolchain uses the old SEQUENCE_free() API
 * taking an `int contents_only` argument (0 = free the whole struct via
 * FREEMEM(), !=0 = free contents only and zero the struct), while OAI source
 * code uses the newer symbolic enum names. Map the symbolic names to the
 * matching integer values so legacy OAI sources compile unchanged.
 *   ASFM_FREE_EVERYTHING (0) -> FREEMEM(sptr): free structure and contents
 *   ASFM_FREE_UNDERLYING (1) -> free contents, keep/zero structure
 *   ASFM_FREE_NOTHING    (2) -> free nothing
 * Newer asn1c also defines ASFM_FREE_UNDERLYING_AND_RESET (free contents and
 * zero the struct), which has the same effect as ASFM_FREE_UNDERLYING under
 * the old contents_only API, so alias it to 1.
 */
#ifndef ASFM_FREE_EVERYTHING
#define ASFM_FREE_EVERYTHING 0
#endif
#ifndef ASFM_FREE_UNDERLYING
#define ASFM_FREE_UNDERLYING 1
#endif
#ifndef ASFM_FREE_NOTHING
#define ASFM_FREE_NOTHING 2
#endif
#ifndef ASFM_FREE_UNDERLYING_AND_RESET
#define ASFM_FREE_UNDERLYING_AND_RESET ASFM_FREE_UNDERLYING
#endif
#ifndef ASFM_FREE_CONTENTS
#define ASFM_FREE_CONTENTS ASFM_FREE_UNDERLYING
#endif

#endif
