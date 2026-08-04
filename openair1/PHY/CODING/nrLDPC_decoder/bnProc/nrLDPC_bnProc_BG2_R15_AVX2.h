static inline void nrLDPC_bnProc_BG2_R15_AVX2(int8_t* bnProcBuf,int8_t* bnProcBufRes,  int8_t* llrRes, uint16_t Z  ) {
        uint32_t M, i; 
// Process group with 2 CNs 
// Process group with 3 CNs 
// Process group with 4 CNs 
// Process group with 5 CNs 
       M = (2*Z + 31)>>5;
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[456 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[456 + i ], ((__m256i*) bnProcBuf)[456 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[480 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[456 + i ], ((__m256i*) bnProcBuf)[480 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[504 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[456 + i ], ((__m256i*) bnProcBuf)[504 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[528 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[456 + i ], ((__m256i*) bnProcBuf)[528 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[552 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[456 + i ], ((__m256i*) bnProcBuf)[552 + i]);
}
// Process group with 6 CNs 
       M = (1*Z + 31)>>5;
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[576 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[480 + i ], ((__m256i*) bnProcBuf)[576 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[588 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[480 + i ], ((__m256i*) bnProcBuf)[588 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[600 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[480 + i ], ((__m256i*) bnProcBuf)[600 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[612 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[480 + i ], ((__m256i*) bnProcBuf)[612 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[624 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[480 + i ], ((__m256i*) bnProcBuf)[624 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[636 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[480 + i ], ((__m256i*) bnProcBuf)[636 + i]);
}
// Process group with 7 CNs 
       M = (1*Z + 31)>>5;
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[648 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[492 + i ], ((__m256i*) bnProcBuf)[648 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[660 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[492 + i ], ((__m256i*) bnProcBuf)[660 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[672 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[492 + i ], ((__m256i*) bnProcBuf)[672 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[684 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[492 + i ], ((__m256i*) bnProcBuf)[684 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[696 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[492 + i ], ((__m256i*) bnProcBuf)[696 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[708 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[492 + i ], ((__m256i*) bnProcBuf)[708 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[720 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[492 + i ], ((__m256i*) bnProcBuf)[720 + i]);
}
// Process group with 8 CNs 
       M = (1*Z + 31)>>5;
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[732 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[504 + i ], ((__m256i*) bnProcBuf)[732 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[744 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[504 + i ], ((__m256i*) bnProcBuf)[744 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[756 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[504 + i ], ((__m256i*) bnProcBuf)[756 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[768 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[504 + i ], ((__m256i*) bnProcBuf)[768 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[780 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[504 + i ], ((__m256i*) bnProcBuf)[780 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[792 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[504 + i ], ((__m256i*) bnProcBuf)[792 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[804 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[504 + i ], ((__m256i*) bnProcBuf)[804 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[816 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[504 + i ], ((__m256i*) bnProcBuf)[816 + i]);
}
// Process group with 9 CNs 
       M = (2*Z + 31)>>5;
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[828 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[516 + i ], ((__m256i*) bnProcBuf)[828 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[852 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[516 + i ], ((__m256i*) bnProcBuf)[852 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[876 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[516 + i ], ((__m256i*) bnProcBuf)[876 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[900 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[516 + i ], ((__m256i*) bnProcBuf)[900 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[924 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[516 + i ], ((__m256i*) bnProcBuf)[924 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[948 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[516 + i ], ((__m256i*) bnProcBuf)[948 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[972 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[516 + i ], ((__m256i*) bnProcBuf)[972 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[996 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[516 + i ], ((__m256i*) bnProcBuf)[996 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1020 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[516 + i ], ((__m256i*) bnProcBuf)[1020 + i]);
}
// Process group with 10 CNs 
       M = (1*Z + 31)>>5;
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1044 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[540 + i ], ((__m256i*) bnProcBuf)[1044 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1056 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[540 + i ], ((__m256i*) bnProcBuf)[1056 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1068 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[540 + i ], ((__m256i*) bnProcBuf)[1068 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1080 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[540 + i ], ((__m256i*) bnProcBuf)[1080 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1092 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[540 + i ], ((__m256i*) bnProcBuf)[1092 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1104 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[540 + i ], ((__m256i*) bnProcBuf)[1104 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1116 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[540 + i ], ((__m256i*) bnProcBuf)[1116 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1128 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[540 + i ], ((__m256i*) bnProcBuf)[1128 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1140 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[540 + i ], ((__m256i*) bnProcBuf)[1140 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1152 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[540 + i ], ((__m256i*) bnProcBuf)[1152 + i]);
}
// Process group with 11 CNs 
// Process group with 12 CNs 
       M = (1*Z + 31)>>5;
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1164 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[552 + i ], ((__m256i*) bnProcBuf)[1164 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1176 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[552 + i ], ((__m256i*) bnProcBuf)[1176 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1188 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[552 + i ], ((__m256i*) bnProcBuf)[1188 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1200 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[552 + i ], ((__m256i*) bnProcBuf)[1200 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1212 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[552 + i ], ((__m256i*) bnProcBuf)[1212 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1224 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[552 + i ], ((__m256i*) bnProcBuf)[1224 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1236 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[552 + i ], ((__m256i*) bnProcBuf)[1236 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1248 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[552 + i ], ((__m256i*) bnProcBuf)[1248 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1260 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[552 + i ], ((__m256i*) bnProcBuf)[1260 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1272 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[552 + i ], ((__m256i*) bnProcBuf)[1272 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1284 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[552 + i ], ((__m256i*) bnProcBuf)[1284 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1296 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[552 + i ], ((__m256i*) bnProcBuf)[1296 + i]);
}
// Process group with 13 CNs 
       M = (1*Z + 31)>>5;
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1308 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[564 + i ], ((__m256i*) bnProcBuf)[1308 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1320 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[564 + i ], ((__m256i*) bnProcBuf)[1320 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1332 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[564 + i ], ((__m256i*) bnProcBuf)[1332 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1344 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[564 + i ], ((__m256i*) bnProcBuf)[1344 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1356 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[564 + i ], ((__m256i*) bnProcBuf)[1356 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1368 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[564 + i ], ((__m256i*) bnProcBuf)[1368 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1380 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[564 + i ], ((__m256i*) bnProcBuf)[1380 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1392 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[564 + i ], ((__m256i*) bnProcBuf)[1392 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1404 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[564 + i ], ((__m256i*) bnProcBuf)[1404 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1416 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[564 + i ], ((__m256i*) bnProcBuf)[1416 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1428 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[564 + i ], ((__m256i*) bnProcBuf)[1428 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1440 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[564 + i ], ((__m256i*) bnProcBuf)[1440 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1452 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[564 + i ], ((__m256i*) bnProcBuf)[1452 + i]);
}
// Process group with 14 CNs 
       M = (1*Z + 31)>>5;
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1464 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[576 + i ], ((__m256i*) bnProcBuf)[1464 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1476 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[576 + i ], ((__m256i*) bnProcBuf)[1476 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1488 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[576 + i ], ((__m256i*) bnProcBuf)[1488 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1500 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[576 + i ], ((__m256i*) bnProcBuf)[1500 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1512 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[576 + i ], ((__m256i*) bnProcBuf)[1512 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1524 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[576 + i ], ((__m256i*) bnProcBuf)[1524 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1536 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[576 + i ], ((__m256i*) bnProcBuf)[1536 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1548 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[576 + i ], ((__m256i*) bnProcBuf)[1548 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1560 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[576 + i ], ((__m256i*) bnProcBuf)[1560 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1572 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[576 + i ], ((__m256i*) bnProcBuf)[1572 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1584 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[576 + i ], ((__m256i*) bnProcBuf)[1584 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1596 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[576 + i ], ((__m256i*) bnProcBuf)[1596 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1608 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[576 + i ], ((__m256i*) bnProcBuf)[1608 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1620 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[576 + i ], ((__m256i*) bnProcBuf)[1620 + i]);
}
// Process group with 15 CNs 
// Process group with 16 CNs 
       M = (1*Z + 31)>>5;
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1632 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[588 + i ], ((__m256i*) bnProcBuf)[1632 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1644 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[588 + i ], ((__m256i*) bnProcBuf)[1644 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1656 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[588 + i ], ((__m256i*) bnProcBuf)[1656 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1668 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[588 + i ], ((__m256i*) bnProcBuf)[1668 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1680 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[588 + i ], ((__m256i*) bnProcBuf)[1680 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1692 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[588 + i ], ((__m256i*) bnProcBuf)[1692 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1704 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[588 + i ], ((__m256i*) bnProcBuf)[1704 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1716 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[588 + i ], ((__m256i*) bnProcBuf)[1716 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1728 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[588 + i ], ((__m256i*) bnProcBuf)[1728 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1740 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[588 + i ], ((__m256i*) bnProcBuf)[1740 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1752 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[588 + i ], ((__m256i*) bnProcBuf)[1752 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1764 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[588 + i ], ((__m256i*) bnProcBuf)[1764 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1776 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[588 + i ], ((__m256i*) bnProcBuf)[1776 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1788 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[588 + i ], ((__m256i*) bnProcBuf)[1788 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1800 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[588 + i ], ((__m256i*) bnProcBuf)[1800 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1812 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[588 + i ], ((__m256i*) bnProcBuf)[1812 + i]);
}
// Process group with 17 CNs 
// Process group with 18 CNs 
// Process group with 19 CNs 
// Process group with 20 CNs 
// Process group with 21 CNs 
// Process group with 22 CNs 
       M = (1*Z + 31)>>5;
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1824 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[600 + i ], ((__m256i*) bnProcBuf)[1824 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1836 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[600 + i ], ((__m256i*) bnProcBuf)[1836 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1848 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[600 + i ], ((__m256i*) bnProcBuf)[1848 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1860 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[600 + i ], ((__m256i*) bnProcBuf)[1860 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1872 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[600 + i ], ((__m256i*) bnProcBuf)[1872 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1884 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[600 + i ], ((__m256i*) bnProcBuf)[1884 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1896 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[600 + i ], ((__m256i*) bnProcBuf)[1896 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1908 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[600 + i ], ((__m256i*) bnProcBuf)[1908 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1920 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[600 + i ], ((__m256i*) bnProcBuf)[1920 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1932 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[600 + i ], ((__m256i*) bnProcBuf)[1932 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1944 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[600 + i ], ((__m256i*) bnProcBuf)[1944 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1956 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[600 + i ], ((__m256i*) bnProcBuf)[1956 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1968 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[600 + i ], ((__m256i*) bnProcBuf)[1968 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1980 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[600 + i ], ((__m256i*) bnProcBuf)[1980 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1992 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[600 + i ], ((__m256i*) bnProcBuf)[1992 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2004 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[600 + i ], ((__m256i*) bnProcBuf)[2004 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2016 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[600 + i ], ((__m256i*) bnProcBuf)[2016 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2028 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[600 + i ], ((__m256i*) bnProcBuf)[2028 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2040 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[600 + i ], ((__m256i*) bnProcBuf)[2040 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2052 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[600 + i ], ((__m256i*) bnProcBuf)[2052 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2064 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[600 + i ], ((__m256i*) bnProcBuf)[2064 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2076 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[600 + i ], ((__m256i*) bnProcBuf)[2076 + i]);
}
// Process group with <23 CNs 
       M = (1*Z + 31)>>5;
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2088 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[612 + i ], ((__m256i*) bnProcBuf)[2088 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2100 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[612 + i ], ((__m256i*) bnProcBuf)[2100 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2112 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[612 + i ], ((__m256i*) bnProcBuf)[2112 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2124 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[612 + i ], ((__m256i*) bnProcBuf)[2124 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2136 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[612 + i ], ((__m256i*) bnProcBuf)[2136 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2148 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[612 + i ], ((__m256i*) bnProcBuf)[2148 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2160 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[612 + i ], ((__m256i*) bnProcBuf)[2160 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2172 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[612 + i ], ((__m256i*) bnProcBuf)[2172 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2184 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[612 + i ], ((__m256i*) bnProcBuf)[2184 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2196 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[612 + i ], ((__m256i*) bnProcBuf)[2196 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2208 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[612 + i ], ((__m256i*) bnProcBuf)[2208 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2220 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[612 + i ], ((__m256i*) bnProcBuf)[2220 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2232 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[612 + i ], ((__m256i*) bnProcBuf)[2232 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2244 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[612 + i ], ((__m256i*) bnProcBuf)[2244 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2256 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[612 + i ], ((__m256i*) bnProcBuf)[2256 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2268 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[612 + i ], ((__m256i*) bnProcBuf)[2268 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2280 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[612 + i ], ((__m256i*) bnProcBuf)[2280 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2292 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[612 + i ], ((__m256i*) bnProcBuf)[2292 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2304 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[612 + i ], ((__m256i*) bnProcBuf)[2304 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2316 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[612 + i ], ((__m256i*) bnProcBuf)[2316 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2328 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[612 + i ], ((__m256i*) bnProcBuf)[2328 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2340 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[612 + i ], ((__m256i*) bnProcBuf)[2340 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2352 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[612 + i ], ((__m256i*) bnProcBuf)[2352 + i]);
}
// Process group with 24 CNs 
// Process group with 25 CNs 
// Process group with 26 CNs 
// Process group with 27 CNs 
// Process group with 28 CNs 
// Process group with 29 CNs 
// Process group with 30 CNs 
}
