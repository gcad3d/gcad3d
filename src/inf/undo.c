/* ../inf/undo.c

================================================================== */
void INF_UNDO__(){                   /*! \code
INF_UNDO__        undo creation of geom-obj

see also INF_Source-record (example create with undo)

Source: ../xa/xa_undo.c                    UNDO_...

---------------------------------------------------
TODO:
- commentlines of apps cannot be deleted; make "BLOCK APP .."
- all apps/plugins/datablocks? should start output with:
#BLOCK APP <blockName>
.. # outpt ..
#BLOCKEND APP <blockName>

- Undo-record for this Block (active):
undoTab[.] lNr=<lNr> grpNr=0 u1=a u2=B
- set char u2 to 'B' (block)
- Undo-record for this Block (inactive):
undoTab[.] lNr=<lNr> grpNr=0 u1=d u2=B
---------------------------------------------------


================================================================== \endcode */}
// eof
