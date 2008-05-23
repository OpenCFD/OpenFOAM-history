/*
 * Copyright 2001, Regents of the University of Minnesota
 *
 * timer.c
 *
 * This file contain various timing routines
 *
 * George
 */

#include "parmetis.h"


/*************************************************************************
* This function initializes the various timers
**************************************************************************/
void InitTimers(CtrlType *ctrl)
{
  cleartimer(ctrl->TotalTmr); 
  cleartimer(ctrl->InitPartTmr);
  cleartimer(ctrl->MatchTmr); 
  cleartimer(ctrl->ContractTmr); 
  cleartimer(ctrl->CoarsenTmr); 
  cleartimer(ctrl->RefTmr);
  cleartimer(ctrl->SetupTmr); 
  cleartimer(ctrl->ProjectTmr); 
  cleartimer(ctrl->KWayInitTmr); 
  cleartimer(ctrl->KWayTmr);
  cleartimer(ctrl->MoveTmr);
  cleartimer(ctrl->RemapTmr);

  cleartimer(ctrl->AuxTmr1); 
  cleartimer(ctrl->AuxTmr2); 
  cleartimer(ctrl->AuxTmr3);
  cleartimer(ctrl->AuxTmr4); 
  cleartimer(ctrl->AuxTmr5); 
  cleartimer(ctrl->AuxTmr6);
}


/*************************************************************************
* This function prints timing information about KMETIS
**************************************************************************/
void PrintTimingInfo(CtrlType *ctrl)
{
/*  PrintTimer(ctrl, ctrl->CoarsenTmr,  " Coarsening"); */
  PrintTimer(ctrl, ctrl->SetupTmr,    "      Setup");
  PrintTimer(ctrl, ctrl->MatchTmr,    "   Matching");
  PrintTimer(ctrl, ctrl->ContractTmr, "Contraction");
  PrintTimer(ctrl, ctrl->InitPartTmr, "   InitPart");
/*  PrintTimer(ctrl, ctrl->RefTmr,      " Refinement"); */
  PrintTimer(ctrl, ctrl->ProjectTmr,  "    Project");
  PrintTimer(ctrl, ctrl->KWayInitTmr, " Initialize");
  PrintTimer(ctrl, ctrl->KWayTmr,     "      K-way");
  PrintTimer(ctrl, ctrl->MoveTmr,     "       Move");
  PrintTimer(ctrl, ctrl->RemapTmr,    "      Remap");
  PrintTimer(ctrl, ctrl->TotalTmr,    "      Total");
/*
  PrintTimer(ctrl, ctrl->AuxTmr1,     "       Aux1");
  PrintTimer(ctrl, ctrl->AuxTmr2,     "       Aux2");
  PrintTimer(ctrl, ctrl->AuxTmr3,     "       Aux3");
  PrintTimer(ctrl, ctrl->AuxTmr4,     "       Aux4");
  PrintTimer(ctrl, ctrl->AuxTmr5,     "       Aux5");
*/
  PrintTimer(ctrl, ctrl->AuxTmr6,     "       Aux6");
}


/*************************************************************************
* This function prints timer stat
**************************************************************************/
void PrintTimer(CtrlType *ctrl, timer tmr, char *msg)
{
  double sum, max, tsec;

  tsec = gettimer(tmr);
  MPI_Reduce((void *)&tsec, (void *)&sum, 1, MPI_DOUBLE, MPI_SUM, 0, ctrl->comm);

  tsec = gettimer(tmr);
  MPI_Reduce((void *)&tsec, (void *)&max, 1, MPI_DOUBLE, MPI_MAX, 0, ctrl->comm);

  if (ctrl->mype == 0 && sum != 0.0)
    printf("%s: Max: %7.3lf, Sum: %7.3lf, Balance: %7.3lf\n", 
            msg, max, sum, max*ctrl->npes/sum);
}
