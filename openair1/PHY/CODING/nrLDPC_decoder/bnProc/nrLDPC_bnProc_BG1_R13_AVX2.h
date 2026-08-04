static inline void nrLDPC_bnProc_BG1_R13_AVX2(int8_t* bnProcBuf,int8_t* bnProcBufRes,  int8_t* llrRes, uint16_t Z ) {
        uint32_t M, i; 
// Process group with 2 CNs 
// Process group with 3 CNs 
// Process group with 4 CNs 
       M = (1*Z + 31)>>5;
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[504 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[504 + i ], ((__m256i*) bnProcBuf)[504 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[516 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[504 + i ], ((__m256i*) bnProcBuf)[516 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[528 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[504 + i ], ((__m256i*) bnProcBuf)[528 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[540 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[504 + i ], ((__m256i*) bnProcBuf)[540 + i]);
}
// Process group with 5 CNs 
       M = (1*Z + 31)>>5;
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[552 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[516 + i ], ((__m256i*) bnProcBuf)[552 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[564 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[516 + i ], ((__m256i*) bnProcBuf)[564 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[576 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[516 + i ], ((__m256i*) bnProcBuf)[576 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[588 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[516 + i ], ((__m256i*) bnProcBuf)[588 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[600 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[516 + i ], ((__m256i*) bnProcBuf)[600 + i]);
}
// Process group with 6 CNs 
       M = (2*Z + 31)>>5;
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[612 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[528 + i ], ((__m256i*) bnProcBuf)[612 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[636 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[528 + i ], ((__m256i*) bnProcBuf)[636 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[660 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[528 + i ], ((__m256i*) bnProcBuf)[660 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[684 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[528 + i ], ((__m256i*) bnProcBuf)[684 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[708 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[528 + i ], ((__m256i*) bnProcBuf)[708 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[732 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[528 + i ], ((__m256i*) bnProcBuf)[732 + i]);
}
// Process group with 7 CNs 
       M = (4*Z + 31)>>5;
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[756 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[552 + i ], ((__m256i*) bnProcBuf)[756 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[804 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[552 + i ], ((__m256i*) bnProcBuf)[804 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[852 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[552 + i ], ((__m256i*) bnProcBuf)[852 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[900 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[552 + i ], ((__m256i*) bnProcBuf)[900 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[948 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[552 + i ], ((__m256i*) bnProcBuf)[948 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[996 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[552 + i ], ((__m256i*) bnProcBuf)[996 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1044 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[552 + i ], ((__m256i*) bnProcBuf)[1044 + i]);
}
// Process group with 8 CNs 
       M = (3*Z + 31)>>5;
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1092 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[600 + i ], ((__m256i*) bnProcBuf)[1092 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1128 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[600 + i ], ((__m256i*) bnProcBuf)[1128 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1164 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[600 + i ], ((__m256i*) bnProcBuf)[1164 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1200 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[600 + i ], ((__m256i*) bnProcBuf)[1200 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1236 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[600 + i ], ((__m256i*) bnProcBuf)[1236 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1272 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[600 + i ], ((__m256i*) bnProcBuf)[1272 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1308 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[600 + i ], ((__m256i*) bnProcBuf)[1308 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1344 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[600 + i ], ((__m256i*) bnProcBuf)[1344 + i]);
}
// Process group with 9 CNs 
       M = (1*Z + 31)>>5;
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1380 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[636 + i ], ((__m256i*) bnProcBuf)[1380 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1392 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[636 + i ], ((__m256i*) bnProcBuf)[1392 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1404 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[636 + i ], ((__m256i*) bnProcBuf)[1404 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1416 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[636 + i ], ((__m256i*) bnProcBuf)[1416 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1428 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[636 + i ], ((__m256i*) bnProcBuf)[1428 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1440 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[636 + i ], ((__m256i*) bnProcBuf)[1440 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1452 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[636 + i ], ((__m256i*) bnProcBuf)[1452 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1464 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[636 + i ], ((__m256i*) bnProcBuf)[1464 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1476 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[636 + i ], ((__m256i*) bnProcBuf)[1476 + i]);
}
// Process group with 10 CNs 
       M = (4*Z + 31)>>5;
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1488 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[648 + i ], ((__m256i*) bnProcBuf)[1488 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1536 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[648 + i ], ((__m256i*) bnProcBuf)[1536 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1584 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[648 + i ], ((__m256i*) bnProcBuf)[1584 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1632 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[648 + i ], ((__m256i*) bnProcBuf)[1632 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1680 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[648 + i ], ((__m256i*) bnProcBuf)[1680 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1728 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[648 + i ], ((__m256i*) bnProcBuf)[1728 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1776 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[648 + i ], ((__m256i*) bnProcBuf)[1776 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1824 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[648 + i ], ((__m256i*) bnProcBuf)[1824 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1872 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[648 + i ], ((__m256i*) bnProcBuf)[1872 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1920 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[648 + i ], ((__m256i*) bnProcBuf)[1920 + i]);
}
// Process group with 11 CNs 
       M = (3*Z + 31)>>5;
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[1968 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[696 + i ], ((__m256i*) bnProcBuf)[1968 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2004 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[696 + i ], ((__m256i*) bnProcBuf)[2004 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2040 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[696 + i ], ((__m256i*) bnProcBuf)[2040 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2076 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[696 + i ], ((__m256i*) bnProcBuf)[2076 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2112 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[696 + i ], ((__m256i*) bnProcBuf)[2112 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2148 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[696 + i ], ((__m256i*) bnProcBuf)[2148 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2184 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[696 + i ], ((__m256i*) bnProcBuf)[2184 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2220 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[696 + i ], ((__m256i*) bnProcBuf)[2220 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2256 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[696 + i ], ((__m256i*) bnProcBuf)[2256 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2292 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[696 + i ], ((__m256i*) bnProcBuf)[2292 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2328 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[696 + i ], ((__m256i*) bnProcBuf)[2328 + i]);
}
// Process group with 12 CNs 
       M = (4*Z + 31)>>5;
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2364 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[732 + i ], ((__m256i*) bnProcBuf)[2364 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2412 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[732 + i ], ((__m256i*) bnProcBuf)[2412 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2460 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[732 + i ], ((__m256i*) bnProcBuf)[2460 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2508 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[732 + i ], ((__m256i*) bnProcBuf)[2508 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2556 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[732 + i ], ((__m256i*) bnProcBuf)[2556 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2604 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[732 + i ], ((__m256i*) bnProcBuf)[2604 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2652 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[732 + i ], ((__m256i*) bnProcBuf)[2652 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2700 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[732 + i ], ((__m256i*) bnProcBuf)[2700 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2748 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[732 + i ], ((__m256i*) bnProcBuf)[2748 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2796 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[732 + i ], ((__m256i*) bnProcBuf)[2796 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2844 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[732 + i ], ((__m256i*) bnProcBuf)[2844 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2892 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[732 + i ], ((__m256i*) bnProcBuf)[2892 + i]);
}
// Process group with 13 CNs 
       M = (1*Z + 31)>>5;
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2940 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[780 + i ], ((__m256i*) bnProcBuf)[2940 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2952 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[780 + i ], ((__m256i*) bnProcBuf)[2952 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2964 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[780 + i ], ((__m256i*) bnProcBuf)[2964 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2976 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[780 + i ], ((__m256i*) bnProcBuf)[2976 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[2988 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[780 + i ], ((__m256i*) bnProcBuf)[2988 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3000 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[780 + i ], ((__m256i*) bnProcBuf)[3000 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3012 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[780 + i ], ((__m256i*) bnProcBuf)[3012 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3024 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[780 + i ], ((__m256i*) bnProcBuf)[3024 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3036 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[780 + i ], ((__m256i*) bnProcBuf)[3036 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3048 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[780 + i ], ((__m256i*) bnProcBuf)[3048 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3060 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[780 + i ], ((__m256i*) bnProcBuf)[3060 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3072 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[780 + i ], ((__m256i*) bnProcBuf)[3072 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3084 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[780 + i ], ((__m256i*) bnProcBuf)[3084 + i]);
}
// Process group with 14 CNs 
// Process group with 15 CNs 
// Process group with 16 CNs 
// Process group with 17 CNs 
// Process group with 18 CNs 
// Process group with 19 CNs 
// Process group with 20 CNs 
// Process group with 21 CNs 
// Process group with 22 CNs 
// Process group with <23 CNs 
// Process group with 24 CNs 
// Process group with 25 CNs 
// Process group with 26 CNs 
// Process group with 27 CNs 
// Process group with 28 CNs 
       M = (1*Z + 31)>>5;
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3096 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[792 + i ], ((__m256i*) bnProcBuf)[3096 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3108 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[792 + i ], ((__m256i*) bnProcBuf)[3108 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3120 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[792 + i ], ((__m256i*) bnProcBuf)[3120 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3132 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[792 + i ], ((__m256i*) bnProcBuf)[3132 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3144 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[792 + i ], ((__m256i*) bnProcBuf)[3144 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3156 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[792 + i ], ((__m256i*) bnProcBuf)[3156 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3168 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[792 + i ], ((__m256i*) bnProcBuf)[3168 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3180 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[792 + i ], ((__m256i*) bnProcBuf)[3180 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3192 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[792 + i ], ((__m256i*) bnProcBuf)[3192 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3204 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[792 + i ], ((__m256i*) bnProcBuf)[3204 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3216 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[792 + i ], ((__m256i*) bnProcBuf)[3216 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3228 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[792 + i ], ((__m256i*) bnProcBuf)[3228 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3240 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[792 + i ], ((__m256i*) bnProcBuf)[3240 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3252 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[792 + i ], ((__m256i*) bnProcBuf)[3252 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3264 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[792 + i ], ((__m256i*) bnProcBuf)[3264 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3276 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[792 + i ], ((__m256i*) bnProcBuf)[3276 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3288 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[792 + i ], ((__m256i*) bnProcBuf)[3288 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3300 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[792 + i ], ((__m256i*) bnProcBuf)[3300 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3312 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[792 + i ], ((__m256i*) bnProcBuf)[3312 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3324 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[792 + i ], ((__m256i*) bnProcBuf)[3324 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3336 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[792 + i ], ((__m256i*) bnProcBuf)[3336 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3348 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[792 + i ], ((__m256i*) bnProcBuf)[3348 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3360 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[792 + i ], ((__m256i*) bnProcBuf)[3360 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3372 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[792 + i ], ((__m256i*) bnProcBuf)[3372 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3384 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[792 + i ], ((__m256i*) bnProcBuf)[3384 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3396 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[792 + i ], ((__m256i*) bnProcBuf)[3396 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3408 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[792 + i ], ((__m256i*) bnProcBuf)[3408 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3420 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[792 + i ], ((__m256i*) bnProcBuf)[3420 + i]);
}
// Process group with 29 CNs 
// Process group with 30 CNs 
       M = (1*Z + 31)>>5;
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3432 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[804 + i ], ((__m256i*) bnProcBuf)[3432 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3444 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[804 + i ], ((__m256i*) bnProcBuf)[3444 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3456 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[804 + i ], ((__m256i*) bnProcBuf)[3456 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3468 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[804 + i ], ((__m256i*) bnProcBuf)[3468 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3480 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[804 + i ], ((__m256i*) bnProcBuf)[3480 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3492 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[804 + i ], ((__m256i*) bnProcBuf)[3492 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3504 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[804 + i ], ((__m256i*) bnProcBuf)[3504 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3516 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[804 + i ], ((__m256i*) bnProcBuf)[3516 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3528 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[804 + i ], ((__m256i*) bnProcBuf)[3528 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3540 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[804 + i ], ((__m256i*) bnProcBuf)[3540 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3552 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[804 + i ], ((__m256i*) bnProcBuf)[3552 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3564 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[804 + i ], ((__m256i*) bnProcBuf)[3564 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3576 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[804 + i ], ((__m256i*) bnProcBuf)[3576 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3588 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[804 + i ], ((__m256i*) bnProcBuf)[3588 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3600 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[804 + i ], ((__m256i*) bnProcBuf)[3600 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3612 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[804 + i ], ((__m256i*) bnProcBuf)[3612 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3624 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[804 + i ], ((__m256i*) bnProcBuf)[3624 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3636 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[804 + i ], ((__m256i*) bnProcBuf)[3636 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3648 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[804 + i ], ((__m256i*) bnProcBuf)[3648 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3660 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[804 + i ], ((__m256i*) bnProcBuf)[3660 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3672 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[804 + i ], ((__m256i*) bnProcBuf)[3672 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3684 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[804 + i ], ((__m256i*) bnProcBuf)[3684 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3696 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[804 + i ], ((__m256i*) bnProcBuf)[3696 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3708 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[804 + i ], ((__m256i*) bnProcBuf)[3708 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3720 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[804 + i ], ((__m256i*) bnProcBuf)[3720 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3732 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[804 + i ], ((__m256i*) bnProcBuf)[3732 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3744 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[804 + i ], ((__m256i*) bnProcBuf)[3744 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3756 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[804 + i ], ((__m256i*) bnProcBuf)[3756 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3768 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[804 + i ], ((__m256i*) bnProcBuf)[3768 + i]);
}
            for (i=0;i<M;i++) {
            ((__m256i*)bnProcBufRes)[3780 + i ] = simde_mm256_subs_epi8(((__m256i*)llrRes)[804 + i ], ((__m256i*) bnProcBuf)[3780 + i]);
}
}
