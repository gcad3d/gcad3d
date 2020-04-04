/* ../inf/surf.c

New Version of Typ_SUTP.  Not yet implemented.
- will use new tesselator MSH2D and have boolean surface-modifications.



================================================================== */
void INF_SUTP(){                   /*! \code


sutp    SUTP_        ObjGX     Typ_SUTP    trimmed,perforated surf (SurTP) INF_SUTP

WHAT IS A TPS-surface:
  - ObjGX typ=Typ_SUTP with all boundaries (1 outer, n inner)
  - boundaries as DB-curves
  - Record[0] = link to support-surf; PLN|CON|SPH|  ..   TOR/SRU/SRV/SBS
  - Record[1] = link to outer boundary (DB-obj - typ,index)
  - Record[2-] - links to inner boundaries

Functions:
APT_decode_sutp_pln
SUTP_decode_sSur





================================================================== \endcode */}
// eof
