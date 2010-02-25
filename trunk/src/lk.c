/*

PhyML:  a program that  computes maximum likelihood phylogenies from
DNA or AA homologous sequences.

Copyright (C) Stephane Guindon. Oct 2003 onward.

All parts of the source except where indicated are distributed under
the GNU public licence. See http://www.opensource.org for details.

*/

#include "lk.h"


/* int    LIM_SCALE; */
/* phydbl LIM_SCALE_VAL; */
/* phydbl BIG; */
/* phydbl SMALL; */

/*********************************************************/

void Init_Tips_At_One_Site_Nucleotides_Float(char state, int pos, phydbl *p_lk)
{
  switch(state)
    {
    case 'A' : p_lk[pos+0]=1.; p_lk[pos+1]=p_lk[pos+2]=p_lk[pos+3]=.0;
      break;
    case 'C' : p_lk[pos+1]=1.; p_lk[pos+0]=p_lk[pos+2]=p_lk[pos+3]=.0;
      break;
    case 'G' : p_lk[pos+2]=1.; p_lk[pos+1]=p_lk[pos+0]=p_lk[pos+3]=.0;
      break;
    case 'T' : p_lk[pos+3]=1.; p_lk[pos+1]=p_lk[pos+2]=p_lk[pos+0]=.0;
      break;
    case 'U' : p_lk[pos+3]=1.; p_lk[pos+1]=p_lk[pos+2]=p_lk[pos+0]=.0;
      break;
    case 'M' : p_lk[pos+0]=p_lk[pos+1]=1.; p_lk[pos+2]=p_lk[pos+3]=.0;
      break;
    case 'R' : p_lk[pos+0]=p_lk[pos+2]=1.; p_lk[pos+1]=p_lk[pos+3]=.0;
      break;
    case 'W' : p_lk[pos+0]=p_lk[pos+3]=1.; p_lk[pos+1]=p_lk[pos+2]=.0;
      break;
    case 'S' : p_lk[pos+1]=p_lk[pos+2]=1.; p_lk[pos+0]=p_lk[pos+3]=.0;
      break;
    case 'Y' : p_lk[pos+1]=p_lk[pos+3]=1.; p_lk[pos+0]=p_lk[pos+2]=.0;
      break;
    case 'K' : p_lk[pos+2]=p_lk[pos+3]=1.; p_lk[pos+0]=p_lk[pos+1]=.0;
      break;
    case 'B' : p_lk[pos+1]=p_lk[pos+2]=p_lk[pos+3]=1.; p_lk[pos+0]=.0;
      break;
    case 'D' : p_lk[pos+0]=p_lk[pos+2]=p_lk[pos+3]=1.; p_lk[pos+1]=.0;
      break;
    case 'H' : p_lk[pos+0]=p_lk[pos+1]=p_lk[pos+3]=1.; p_lk[pos+2]=.0;
      break;
    case 'V' : p_lk[pos+0]=p_lk[pos+1]=p_lk[pos+2]=1.; p_lk[pos+3]=.0;
      break;
    case 'N' : case 'X' : case '?' : case 'O' : case '-' :
      p_lk[pos+0]=p_lk[pos+1]=p_lk[pos+2]=p_lk[pos+3]=1.;break;
    default :
      {
	PhyML_Printf("\n. Unknown character state : %c\n",state);
	Exit("\n. Init failed (check the data type)\n");
	break;
      }
    }
}

/*********************************************************/

void Init_Tips_At_One_Site_Nucleotides_Int(char state, int pos, short int *p_pars)
{
  switch(state)
    {
    case 'A' : p_pars[pos+0]=1; p_pars[pos+1]=p_pars[pos+2]=p_pars[pos+3]=0;
      break;
    case 'C' : p_pars[pos+1]=1; p_pars[pos+0]=p_pars[pos+2]=p_pars[pos+3]=0;
      break;
    case 'G' : p_pars[pos+2]=1; p_pars[pos+1]=p_pars[pos+0]=p_pars[pos+3]=0;
      break;
    case 'T' : p_pars[pos+3]=1; p_pars[pos+1]=p_pars[pos+2]=p_pars[pos+0]=0;
      break;
    case 'U' : p_pars[pos+3]=1; p_pars[pos+1]=p_pars[pos+2]=p_pars[pos+0]=0;
      break;
    case 'M' : p_pars[pos+0]=p_pars[pos+1]=1; p_pars[pos+2]=p_pars[pos+3]=0;
      break;
    case 'R' : p_pars[pos+0]=p_pars[pos+2]=1; p_pars[pos+1]=p_pars[pos+3]=0;
      break;
    case 'W' : p_pars[pos+0]=p_pars[pos+3]=1; p_pars[pos+1]=p_pars[pos+2]=0;
      break;
    case 'S' : p_pars[pos+1]=p_pars[pos+2]=1; p_pars[pos+0]=p_pars[pos+3]=0;
      break;
    case 'Y' : p_pars[pos+1]=p_pars[pos+3]=1; p_pars[pos+0]=p_pars[pos+2]=0;
      break;
    case 'K' : p_pars[pos+2]=p_pars[pos+3]=1; p_pars[pos+0]=p_pars[pos+1]=0;
      break;
    case 'B' : p_pars[pos+1]=p_pars[pos+2]=p_pars[pos+3]=1; p_pars[pos+0]=0;
      break;
    case 'D' : p_pars[pos+0]=p_pars[pos+2]=p_pars[pos+3]=1; p_pars[pos+1]=0;
      break;
    case 'H' : p_pars[pos+0]=p_pars[pos+1]=p_pars[pos+3]=1; p_pars[pos+2]=0;
      break;
    case 'V' : p_pars[pos+0]=p_pars[pos+1]=p_pars[pos+2]=1; p_pars[pos+3]=0;
      break;
    case 'N' : case 'X' : case '?' : case 'O' : case '-' :
      p_pars[pos+0]=p_pars[pos+1]=p_pars[pos+2]=p_pars[pos+3]=1;break;
    default :
      {
	PhyML_Printf("\n. Unknown character state : %c\n",state);
	Exit("\n. Init failed (check the data type)\n");
	break;
      }
    }
}

/*********************************************************/

void Init_Tips_At_One_Site_AA_Float(char aa, int pos, phydbl *p_lk)
{
  int i;

  For(i,20) p_lk[pos+i] = .0;

  switch(aa){
  case 'A' : p_lk[pos+0]= 1.; break;/* Alanine */
  case 'R' : p_lk[pos+1]= 1.; break;/* Arginine */
  case 'N' : p_lk[pos+2]= 1.; break;/* Asparagine */
  case 'D' : p_lk[pos+3]= 1.; break;/* Aspartic acid */
  case 'C' : p_lk[pos+4]= 1.; break;/* Cysteine */
  case 'Q' : p_lk[pos+5]= 1.; break;/* Glutamine */
  case 'E' : p_lk[pos+6]= 1.; break;/* Glutamic acid */
  case 'G' : p_lk[pos+7]= 1.; break;/* Glycine */
  case 'H' : p_lk[pos+8]= 1.; break;/* Histidine */
  case 'I' : p_lk[pos+9]= 1.; break;/* Isoleucine */
  case 'L' : p_lk[pos+10]=1.; break;/* Leucine */
  case 'K' : p_lk[pos+11]=1.; break;/* Lysine */
  case 'M' : p_lk[pos+12]=1.; break;/* Methionine */
  case 'F' : p_lk[pos+13]=1.; break;/* Phenylalanin */
  case 'P' : p_lk[pos+14]=1.; break;/* Proline */
  case 'S' : p_lk[pos+15]=1.; break;/* Serine */
  case 'T' : p_lk[pos+16]=1.; break;/* Threonine */
  case 'W' : p_lk[pos+17]=1.; break;/* Tryptophan */
  case 'Y' : p_lk[pos+18]=1.; break;/* Tyrosine */
  case 'V' : p_lk[pos+19]=1.; break;/* Valine */

  case 'B' : p_lk[pos+2]= 1.; break;/* Asparagine */
  case 'Z' : p_lk[pos+5]= 1.; break;/* Glutamine */

  case 'X' : case '?' : case '-' : For(i,20) p_lk[pos+i] = 1.; break;
  default :
    {
      PhyML_Printf("\n. Unknown character state : %c\n",aa);
      Exit("\n. Init failed (check the data type)\n");
      break;
    }
  }
}

/*********************************************************/

void Init_Tips_At_One_Site_AA_Int(char aa, int pos, short int *p_pars)
{
  int i;

  For(i,20) p_pars[pos+i] = .0;

  switch(aa){
  case 'A' : p_pars[pos+0]  = 1; break;/* Alanine */
  case 'R' : p_pars[pos+1]  = 1; break;/* Arginine */
  case 'N' : p_pars[pos+2]  = 1; break;/* Asparagine */
  case 'D' : p_pars[pos+3]  = 1; break;/* Aspartic acid */
  case 'C' : p_pars[pos+4]  = 1; break;/* Cysteine */
  case 'Q' : p_pars[pos+5]  = 1; break;/* Glutamine */
  case 'E' : p_pars[pos+6]  = 1; break;/* Glutamic acid */
  case 'G' : p_pars[pos+7]  = 1; break;/* Glycine */
  case 'H' : p_pars[pos+8]  = 1; break;/* Histidine */
  case 'I' : p_pars[pos+9]  = 1; break;/* Isoleucine */
  case 'L' : p_pars[pos+10] = 1; break;/* Leucine */
  case 'K' : p_pars[pos+11] = 1; break;/* Lysine */
  case 'M' : p_pars[pos+12] = 1; break;/* Methionine */
  case 'F' : p_pars[pos+13] = 1; break;/* Phenylalanin */
  case 'P' : p_pars[pos+14] = 1; break;/* Proline */
  case 'S' : p_pars[pos+15] = 1; break;/* Serine */
  case 'T' : p_pars[pos+16] = 1; break;/* Threonine */
  case 'W' : p_pars[pos+17] = 1; break;/* Tryptophan */
  case 'Y' : p_pars[pos+18] = 1; break;/* Tyrosine */
  case 'V' : p_pars[pos+19] = 1; break;/* Valine */

  case 'B' : p_pars[pos+2]  = 1; break;/* Asparagine */
  case 'Z' : p_pars[pos+5]  = 1; break;/* Glutamine */

  case 'X' : case '?' : case '-' : For(i,20) p_pars[pos+i] = 1; break;
  default :
    {
      PhyML_Printf("\n. Unknown character state : %c\n",aa);
      Exit("\n. Init failed (check the data type)\n");
      break;
    }
  }
}

/*********************************************************/

void Init_Tips_At_One_Site_Generic_Float(char *state, int ns, int state_len, int pos, phydbl *p_lk)
{
  int i;
  int state_int;

  For(i,ns) p_lk[pos+i] = 0.;

  if(Is_Ambigu(state,GENERIC,state_len)) For(i,ns) p_lk[pos+i] = 1.;
  else
    {
      char format[6];
      sprintf(format,"%%%dd",state_len);
      if(!sscanf(state,format,&state_int))
	{
	  PhyML_Printf("\n. state='%c'",state);
	  PhyML_Printf("\n. Err in file %s at line %d\n\n",__FILE__,__LINE__);
	  Warn_And_Exit("");
	}
      if(state_int > ns)
	{
	  PhyML_Printf("\n. %s %d cstate: %.2s istate: %d state_len: %d.\n",__FILE__,__LINE__,state,state_int,state_len);
	  PhyML_Printf("\n. Err in file %s at line %d\n",__FILE__,__LINE__);
	  Warn_And_Exit("");	  
	}
      p_lk[pos+state_int] = 1.;
      /*       PhyML_Printf("\n. %s %d cstate: %.2s istate: %d state_len: %d ns: %d pos: %d",__FILE__,__LINE__,state,state_int,state_len,ns,pos); */
    }
}

/*********************************************************/

void Init_Tips_At_One_Site_Generic_Int(char *state, int ns, int state_len, int pos, short int *p_pars)
{
  int i;
  int state_int;

  For(i,ns) p_pars[pos+i] = 0;
  
  if(Is_Ambigu(state,GENERIC,state_len)) For(i,ns) p_pars[pos+i] = 1;
  else 
    {
      char format[6];
      sprintf(format,"%%%dd",state_len);
      if(!sscanf(state,format,&state_int))
	{
	  PhyML_Printf("\n. state='%c'",state);
	  PhyML_Printf("\n. Err in file %s at line %d\n\n",__FILE__,__LINE__);
	  Warn_And_Exit("");
	}
      if(state_int > ns)
	{
	  PhyML_Printf("\n. %s %d cstate: %.2s istate: %d state_len: %d.\n",__FILE__,__LINE__,state,state_int,state_len);
	  PhyML_Printf("\n. Err in file %s at line %d\n",__FILE__,__LINE__);
	  Warn_And_Exit("");	  
	}
      p_pars[pos+state_int] = 1;
/*       PhyML_Printf("\n* %s %d cstate: %.2s istate: %d state_len: %d ns: %d pos: %d",__FILE__,__LINE__,state,state_int,state_len,ns,pos); */
    }
}

/*********************************************************/

void Get_All_Partial_Lk_Scale(t_tree *tree, t_edge *b_fcus, t_node *a, t_node *d)
{
  if(d->tax) return;
  else Update_P_Lk(tree,b_fcus,d);
}

/*********************************************************/

void Post_Order_Lk(t_node *a, t_node *d, t_tree *tree)
{
  int i,dir;

  dir = -1;

  if(d->tax) return;
  else
    {
      For(i,3)
	{
	  if(d->v[i] != a)
	    Post_Order_Lk(d,d->v[i],tree);
	  else dir = i;
	}      
      Get_All_Partial_Lk_Scale(tree,d->b[dir],a,d);
    }
}

/*********************************************************/

void Pre_Order_Lk(t_node *a, t_node *d, t_tree *tree)
{
  int i;

  if(d->tax) return;
  else
    {
      For(i,3)
	{
	  if(d->v[i] != a)
	    {
	      Get_All_Partial_Lk_Scale(tree,d->b[i],d->v[i],d);
	      Pre_Order_Lk(d,d->v[i],tree);
	    }
	}
    }
}

/*********************************************************/

phydbl Lk(t_tree *tree)
{
  int br;
  int n_patterns;


  tree->old_lnL = tree->c_lnL;

  if(tree->rates && tree->rates->lk_approx == NORMAL)
    {
      tree->c_lnL = Dnorm_Multi_Given_InvCov_Det(tree->rates->u_cur_l,
						 tree->rates->u_ml_l,
						 tree->rates->invcov,
						 tree->rates->covdet,
						 2*tree->n_otu-3,YES);
      return tree->c_lnL;
    }


  n_patterns = tree->n_pattern;

  Set_Model_Parameters(tree->mod);

#ifdef TIMES
  if((tree->rates) && (tree->rates->bl_from_rt)) RATES_Update_Cur_Bl(tree);
  if(tree->bl_from_node_stamps) TIMES_Bl_From_T(tree);
#endif

  For(br,2*tree->n_otu-3) Update_PMat_At_Given_Edge(tree->t_edges[br],tree);

  Post_Order_Lk(tree->noeud[0],tree->noeud[0]->v[0],tree);
  if(tree->both_sides)
    Pre_Order_Lk(tree->noeud[0],
		 tree->noeud[0]->v[0],
		 tree);

  tree->c_lnL             = .0;
  tree->sum_min_sum_scale = .0;
  For(tree->curr_site,n_patterns)
    {
      if(tree->data->wght[tree->curr_site] > SMALL) Lk_Core(tree->noeud[0]->b[0],tree);
    }

/*   Qksort(tree->c_lnL_sorted,NULL,0,n_patterns-1); */

/*   tree->c_lnL = .0; */
/*   For(tree->curr_site,n_patterns) */
/*     { */
/*       tree->c_lnL += tree->c_lnL_sorted[tree->curr_site]; */
/*     } */

    
  Adjust_Min_Diff_Lk(tree);

  return tree->c_lnL;
}

/*********************************************************/

phydbl Lk_At_Given_Edge(t_edge *b_fcus, t_tree *tree)
{
  int n_patterns;
  
  n_patterns = tree->n_pattern;

#ifdef TIMES
  if((tree->rates) && (tree->rates->bl_from_rt)) RATES_Update_Cur_Bl(tree);
  if(tree->bl_from_node_stamps) TIMES_Bl_From_T(tree);
#endif

  Update_PMat_At_Given_Edge(b_fcus,tree);

  if(b_fcus->left->tax)
    {
      PhyML_Printf("\n. Err in file %s at line %d\n\n",__FILE__,__LINE__);
      Warn_And_Exit("");
    }

  tree->c_lnL             = .0;
  tree->sum_min_sum_scale = .0;
  For(tree->curr_site,n_patterns)
    {
      if(tree->data->wght[tree->curr_site] > SMALL) Lk_Core(b_fcus,tree);
    }

/*   Qksort(tree->c_lnL_sorted,NULL,0,n_patterns-1); */

/*   tree->c_lnL = .0; */
/*   For(tree->curr_site,n_patterns) */
/*     { */
/*       tree->c_lnL += tree->c_lnL_sorted[tree->curr_site]; */
/*     } */


  Adjust_Min_Diff_Lk(tree);

  return tree->c_lnL;
}

/*********************************************************/
/* Core of the likelihood calculcation. Assume that the partial likelihoods on both
   sides of t_edge *b are up-to-date. Calculate the log-likelihood at one site.
*/

#ifndef USE_OLD_LK
phydbl Lk_Core(t_edge *b, t_tree *tree)
{
  phydbl log_site_lk;
  phydbl site_lk_cat, site_lk;
  int sum_scale_left, sum_scale_rght;
  int fact_sum_scale;
  phydbl max_sum_scale;
  phydbl sum;
  int ambiguity_check,state;
  int catg,ns,k,l,site;
  int dim1,dim2,dim3;
  int *sum_scale_left_cat,*sum_scale_rght_cat;
  int n_iter;
  phydbl multiplier;
  int exponent, piecewise_exponent;
  phydbl tmp;
  phydbl logbig;
  phydbl inv_site_lk;

  logbig = LOG((phydbl)BIG);

  dim1 = tree->mod->n_catg * tree->mod->ns;
  dim2 = tree->mod->ns;
  dim3 = tree->mod->ns * tree->mod->ns;

  log_site_lk     = .0;
  site_lk         = .0;
  site_lk_cat     = .0;
  ambiguity_check = -1;
  state           = -1;
  site            = tree->curr_site;
  ns              = tree->mod->ns;

  if((b->rght->tax) && (!tree->mod->s_opt->greedy))
    {
      ambiguity_check = tree->data->c_seq[b->rght->num]->is_ambigu[site];
      if(!ambiguity_check) state = Get_State_From_P_Pars(b->p_lk_tip_r,site*dim2,tree);
    }

  sum_scale_left = .0;
  sum_scale_rght = .0;
  
  if(tree->mod->use_m4mod) ambiguity_check = 1;
  
  sum_scale_left_cat = b->sum_scale_left_cat;
  sum_scale_rght_cat = b->sum_scale_rght_cat;


  /* Actual likelihood calculation */
  /* For all classes of rates */
  For(catg,tree->mod->n_catg)
    {
      site_lk_cat = .0;

      /* b is an external edge */
      if((b->rght->tax) && (!tree->mod->s_opt->greedy))
        {
          /* If the character observed at the tip is NOT ambiguous: ns x 1 terms to consider */
          if(!ambiguity_check)
            {
              sum = .0;
              For(l,ns)
                {
                  sum +=
                    b->Pij_rr[catg*dim3+state*dim2+l] *
                    b->p_lk_left[site*dim1+catg*dim2+l];
                }

              site_lk_cat += sum * tree->mod->pi[state];
            }
          /* If the character observed at the tip is ambiguous: ns x ns terms to consider */
          else
            {
              For(k,ns)
                {
                  sum = .0;
                  if(b->p_lk_tip_r[site*dim2+k] > .0)
                    {
                      For(l,ns)
                        {
                          sum +=
                            b->Pij_rr[catg*dim3+k*dim2+l] *
                            b->p_lk_left[site*dim1+catg*dim2+l];
                        }

                      site_lk_cat +=
                        sum *
                        tree->mod->pi[k] *
                        b->p_lk_tip_r[site*dim2+k];		      
                    }
                }
            }
        }
      /* b is an internal edge: ns x ns terms to consider */
      else
        {
          For(k,ns)
            {
              sum = .0;
              if(b->p_lk_rght[site*dim1+catg*dim2+k] > .0)
                {
                  For(l,ns)
                    {
                      sum +=
                        b->Pij_rr[catg*dim3+k*dim2+l] *
                        b->p_lk_left[site*dim1+catg*dim2+l];
                    }

                  site_lk_cat +=
                    sum *
                    tree->mod->pi[k] *
                    b->p_lk_rght[site*dim1+catg*dim2+k];
                }
            }
        }
      tree->site_lk_cat[catg] = site_lk_cat;     
    }
    
  max_sum_scale =  (phydbl)BIG;
  For(catg,tree->mod->n_catg)
    {
      sum_scale_left_cat[catg] =
        (b->sum_scale_left)?
        (b->sum_scale_left[catg*tree->n_pattern+site]):
        (0.0);
      
      sum_scale_rght_cat[catg] =
        (b->sum_scale_rght)?
        (b->sum_scale_rght[catg*tree->n_pattern+site]):
        (0.0);

      sum = sum_scale_left_cat[catg] + sum_scale_rght_cat[catg];

      if(sum < .0)
	{
	  PhyML_Printf("\n. sum = %G",sum);
	  PhyML_Printf("\n. Err in file %s at line %d\n\n",__FILE__,__LINE__);
	  Warn_And_Exit("\n");
	}

      tmp = sum + (logbig - LOG(tree->site_lk_cat[catg]))/(phydbl)LOG2;
      if(tmp < max_sum_scale) max_sum_scale = tmp; /* min of the maxs */
    }

/*   fact_sum_scale = (int)((max_sum_scale + min_sum_scale) / 2); */


  fact_sum_scale = (int)(max_sum_scale / 2);


  /* Apply scaling factors */
  For(catg,tree->mod->n_catg)
    {
      exponent = -(sum_scale_left_cat[catg]+sum_scale_rght_cat[catg])+fact_sum_scale;

      site_lk_cat = tree->site_lk_cat[catg];
      

      if(exponent > 0)
	{
	  do
	    {
	      piecewise_exponent = MIN(exponent,63);
	      multiplier = (phydbl)((unsigned long long)(1) << piecewise_exponent);
	      site_lk_cat *= multiplier;
	      exponent -= piecewise_exponent;
	    }
	  while(exponent != 0);
	}
      else
	{
	  do
	    {
	      piecewise_exponent = MAX(exponent,-63);
	      multiplier = 1. / (phydbl)((unsigned long long)(1) << -piecewise_exponent);
	      site_lk_cat *= multiplier;
	      exponent -= piecewise_exponent;
	    }
	  while(exponent != 0);
	}

      if(isinf(site_lk_cat))
	{
	  PhyML_Printf("\n+ site=%4d site_lk_cat=%G sum_scale=%d max=%G fact=%d expo=%d dbl=%G",
		       tree->curr_site,
		       tree->site_lk_cat[catg],
		       sum_scale_left_cat[catg]+sum_scale_rght_cat[catg],
		       max_sum_scale,
		       fact_sum_scale,
		       -(sum_scale_left_cat[catg]+sum_scale_rght_cat[catg])+fact_sum_scale,
		       (double)tree->site_lk_cat[catg] * pow(2.,-(sum_scale_left_cat[catg]+sum_scale_rght_cat[catg])+fact_sum_scale));
	  site_lk_cat = BIG / 10;
	}

      if(site_lk_cat < SMALL) site_lk_cat = .0;

      tree->site_lk_cat[catg] = site_lk_cat;
    }

  site_lk = .0;
  For(catg,tree->mod->n_catg) site_lk += tree->site_lk_cat[catg] * tree->mod->gamma_r_proba[catg];
  
  /* The substitution model does include invariable sites */
  if(tree->mod->invar)
    {
      /* The site is invariant */
      if(tree->data->invar[site] > -0.5)
        {
	  /* Multiply P(D|r=0) by 2^(fact_sum_scale) */

	  inv_site_lk = tree->mod->pi[tree->data->invar[site]];
	  exponent = fact_sum_scale;
	  do
	    {
	      piecewise_exponent = MIN(exponent,63);
	      multiplier = (phydbl)((unsigned long long)(1) << piecewise_exponent);
	      inv_site_lk *= multiplier;
	      exponent -= piecewise_exponent;
	    }
	  while(exponent != 0);
	  inv_site_lk *= tree->mod->pinvar;
	  /* Update the value of site_lk */
	  site_lk *= (1. - tree->mod->pinvar);
	  site_lk += inv_site_lk;
        }
      else
        {
          /* Same formula as above with P(D | subs, rate = 0) = 0 */
	  site_lk *= (1. - tree->mod->pinvar);
        }
    }
  
  log_site_lk = LOG(site_lk) - (phydbl)LOG2 * fact_sum_scale;

  For(catg,tree->mod->n_catg) tree->log_site_lk_cat[catg][site] = LOG(tree->site_lk_cat[catg]) - (phydbl)LOG2 * fact_sum_scale;
  
  if(isinf(log_site_lk) || isnan(log_site_lk))
    {
      PhyML_Printf("\n. invar = %f",tree->data->invar[site]);
      PhyML_Printf("\n. scale_left = %d scale_rght = %d",sum_scale_left,sum_scale_rght);
      PhyML_Printf("\n. Lk = %G LOG(Lk) = %f < %G",site_lk,log_site_lk,-BIG);
      PhyML_Printf("\n. Err in file %s at line %d\n\n",__FILE__,__LINE__);
      Warn_And_Exit("\n");
    }
  
  tree->cur_site_lk[site] = log_site_lk;

  /* Multiply log likelihood by the number of times this site pattern is found in the data */
  tree->c_lnL_sorted[site] = tree->data->wght[site]*log_site_lk;

  tree->c_lnL += tree->data->wght[site]*log_site_lk;
/*   tree->sum_min_sum_scale += (int)tree->data->wght[site]*min_sum_scale; */
  
  return log_site_lk;
}

/*********************************************************/

/* Update partial likelihood on edge b on the side of b where
   node d lies.
*/
void Update_P_Lk(t_tree *tree, t_edge *b, t_node *d)
{
/*
           |
	   |<- b
	   |
	   d
          / \
       	 /   \
       	/     \
	n_v1   n_v2
*/
  t_node *n_v1, *n_v2;
  phydbl p1_lk1,p2_lk2;
  phydbl *p_lk,*p_lk_v1,*p_lk_v2;
  phydbl *Pij1,*Pij2;
  int *sum_scale, *sum_scale_v1, *sum_scale_v2;
  int sum_scale_v1_val, sum_scale_v2_val;
  int i,j;
  int catg,site;
  int dir1,dir2;
  int n_patterns;
  short int ambiguity_check_v1,ambiguity_check_v2;
  int state_v1,state_v2;
  int dim1, dim2, dim3;
  phydbl curr_scaler;
  int curr_scaler_pow, piecewise_scaler_pow;
  phydbl p_lk_lim_inf;
  phydbl smallest_p_lk;

  p_lk_lim_inf = (phydbl)P_LK_LIM_INF;
 
  dim1 = tree->mod->n_catg * tree->mod->ns;
  dim2 = tree->mod->ns;
  dim3 = tree->mod->ns * tree->mod->ns;

  state_v1 = state_v2 = -1;
  ambiguity_check_v1 = ambiguity_check_v2 = NO;
  sum_scale_v1_val = sum_scale_v2_val = 0;
  p1_lk1 = p2_lk2 = .0;

  if(d->tax)
    {
      PhyML_Printf("\n. t_node %d is a leaf...",d->num);
      PhyML_Printf("\n. Err in file %s at line %d\n\n",__FILE__,__LINE__);
      Warn_And_Exit("\n");
    }

  n_patterns = tree->n_pattern;
  
  /* TO DO: Might be worth keeping these directions in memory instead of
     calculating them every time... */
  dir1=dir2=-1;
  For(i,3) if(d->b[i] != b) (dir1<0)?(dir1=i):(dir2=i);

/*   if((dir1 == -1) || (dir2 == -1)) */
/*     { */
/*       PhyML_Printf("\n. d = %d",d->num); */
/*       PhyML_Printf("\n. d->v[0] = %d, d->v[1] = %d, d->v[2] = %d",d->v[0]->num,d->v[1]->num,d->v[2]->num); */
/*       PhyML_Printf("\n. d->b[0] = %d, d->b[1] = %d, d->b[2] = %d",d->b[0]->num,d->b[1]->num,d->b[2]->num); */
/*       PhyML_Printf("\n. d->num = %d dir1 = %d dir2 = %d",d->num,dir1,dir2); */
/*       PhyML_Printf("\n. Err in file %s at line %d\n\n",__FILE__,__LINE__); */
/*       Exit(""); */
/*     } */
  
  n_v1 = d->v[dir1];
  n_v2 = d->v[dir2];

  /* Get the partial likelihood vectors on edge b and the two pendant
     edges (i.e., the two other edges connected to d) */
  if(d == b->left)
    {
      p_lk = b->p_lk_left;
      sum_scale = b->sum_scale_left;
    }
  else
    {
      p_lk = b->p_lk_rght;
      sum_scale = b->sum_scale_rght;
    }
      
  if(d == d->b[dir1]->left)
    {
      p_lk_v1 = d->b[dir1]->p_lk_rght;
      sum_scale_v1 = d->b[dir1]->sum_scale_rght;
    }
  else
    {
      p_lk_v1 = d->b[dir1]->p_lk_left;
      sum_scale_v1 = d->b[dir1]->sum_scale_left;
    }
  
  if(d == d->b[dir2]->left)
    {
      p_lk_v2 = d->b[dir2]->p_lk_rght;
      sum_scale_v2 = d->b[dir2]->sum_scale_rght;
    }
  else
    {
      p_lk_v2 = d->b[dir2]->p_lk_left;
      sum_scale_v2 = d->b[dir2]->sum_scale_left;
    }
  
  /* Change probability matrices on the two pendant edges */
  Pij1 = d->b[dir1]->Pij_rr;
  Pij2 = d->b[dir2]->Pij_rr;
  
  /* For every site in the alignment */
  For(site,n_patterns)
    {
      state_v1 = state_v2 = -1;
      ambiguity_check_v1 = ambiguity_check_v2 = NO;
      
      if(!tree->mod->s_opt->greedy)
	{
	  /* n_v1 and n_v2 are tip nodes */
	  if(n_v1->tax)
	    {
	      /* Is the state at this tip ambiguous? */
	      ambiguity_check_v1 = tree->data->c_seq[n_v1->num]->is_ambigu[site];
	      if(ambiguity_check_v1 == NO) state_v1 = Get_State_From_P_Pars(n_v1->b[0]->p_lk_tip_r,site*dim2,tree);
	    }
	      
	  if(n_v2->tax)
	    {
	      /* Is the state at this tip ambiguous? */
	      ambiguity_check_v2 = tree->data->c_seq[n_v2->num]->is_ambigu[site];
	      if(ambiguity_check_v2 == NO) state_v2 = Get_State_From_P_Pars(n_v2->b[0]->p_lk_tip_r,site*dim2,tree);
	    }
	}
      
      if(tree->mod->use_m4mod)
	{
	  ambiguity_check_v1 = YES;
	  ambiguity_check_v2 = YES;
	}

      /* For all the rate classes */
      For(catg,tree->mod->n_catg)
	{
	  smallest_p_lk  =  BIG;

	  /* For all the state at node d */
	  For(i,tree->mod->ns)
	    {
	      p1_lk1 = .0;
	      
	      /* n_v1 is a tip */
	      if((n_v1->tax) && (!tree->mod->s_opt->greedy))
		{
		  if(ambiguity_check_v1 == NO)
		    {
		      /* For the (non-ambiguous) state at node n_v1 */
		      p1_lk1 = Pij1[catg*dim3+i*dim2+state_v1];
		    }
		  else
		    {
		      /* For all the states at node n_v1 */
		      For(j,tree->mod->ns)
			{
			  p1_lk1 += Pij1[catg*dim3+i*dim2+j] * (phydbl)n_v1->b[0]->p_lk_tip_r[site*dim2+j];
			}
		    }
		}
	      /* n_v1 is an internal node */
	      else
		{
		  /* For the states at node n_v1 */
		  For(j,tree->mod->ns)
		    {
		      p1_lk1 += Pij1[catg*dim3+i*dim2+j] * p_lk_v1[site*dim1+catg*dim2+j];
		    }
		}
	      
	      p2_lk2 = .0;
	      
	      /* We do exactly the same as for node n_v1 but for node n_v2 this time.*/
	      /* n_v2 is a tip */
	      if((n_v2->tax) && (!tree->mod->s_opt->greedy))
		{
		  if(ambiguity_check_v2 == NO)
		    {
		      /* For the (non-ambiguous) state at node n_v2 */
		      p2_lk2 = Pij2[catg*dim3+i*dim2+state_v2];
		    }
		  else
		    {
		      /* For all the states at node n_v2 */
		      For(j,tree->mod->ns)
			{
			  p2_lk2 += Pij2[catg*dim3+i*dim2+j] * (phydbl)n_v2->b[0]->p_lk_tip_r[site*dim2+j];
			}
		    }
		}
	      /* n_v2 is an internal node */
	      else
		{
		  /* For all the states at node n_v2 */
		  For(j,tree->mod->ns)
		    {
		      p2_lk2 += Pij2[catg*dim3+i*dim2+j] * p_lk_v2[site*dim1+catg*dim2+j];
		    }
		}
	      
	      p_lk[site*dim1+catg*dim2+i] = p1_lk1 * p2_lk2;	    

	      if(p_lk[site*dim1+catg*dim2+i] < smallest_p_lk) smallest_p_lk = p_lk[site*dim1+catg*dim2+i] ; 
	    }
	      
	  /* Current scaling values at that site */
 	  sum_scale_v1_val = (sum_scale_v1)?(sum_scale_v1[catg*n_patterns+site]):(0);
	  sum_scale_v2_val = (sum_scale_v2)?(sum_scale_v2[catg*n_patterns+site]):(0);
	  
	  sum_scale[catg*n_patterns+site] = sum_scale_v1_val + sum_scale_v2_val;
	  
	  /* Scaling */
	  if(smallest_p_lk < p_lk_lim_inf)
	    {
	      curr_scaler_pow = (int)(LOG(p_lk_lim_inf)-LOG(smallest_p_lk))/LOG2;
	      curr_scaler     = (phydbl)((unsigned long long)(1) << curr_scaler_pow);

/* 	      if(fabs(curr_scaler_pow) > 63 || fabs(curr_scaler_pow) > 63) */
/* 		{ */
/* 		  PhyML_Printf("\n. p_lk_lim_inf = %G smallest_p_lk = %G",p_lk_lim_inf,smallest_p_lk); */
/* 		  PhyML_Printf("\n. curr_scaler_pow = %d",curr_scaler_pow); */
/* 		  PhyML_Printf("\n. Err in file %s at line %d.",__FILE__,__LINE__); */
/* 		  Warn_And_Exit("\n"); */
/* 		} */

	      sum_scale[catg*n_patterns+site] += curr_scaler_pow;

	      do
		{
		  piecewise_scaler_pow = MIN(curr_scaler_pow,63);
		  curr_scaler = (phydbl)((unsigned long long)(1) << piecewise_scaler_pow);
		  For(i,tree->mod->ns)
		    {
		      p_lk[site*dim1+catg*dim2+i] *= curr_scaler;
		      
		      if(p_lk[site*dim1+catg*dim2+i] > BIG)
			{
			  PhyML_Printf("\n. p_lk_lim_inf = %G smallest_p_lk = %G",p_lk_lim_inf,smallest_p_lk);
			  PhyML_Printf("\n. curr_scaler_pow = %d",curr_scaler_pow);
			  PhyML_Printf("\n. Err in file %s at line %d.",__FILE__,__LINE__);
			  Warn_And_Exit("\n");
			}
		    }
		  curr_scaler_pow -= piecewise_scaler_pow;
		}
	      while(curr_scaler_pow != 0);
	    }
	}
    }
}

#endif

/*********************************************************/

phydbl Return_Abs_Lk(t_tree *tree)
{
  Lk(tree);
  return FABS(tree->c_lnL);
}

/*********************************************************/

matrix *ML_Dist(calign *data, model *mod)
{
  int i,j,k,l;
  phydbl init;
  int n_catg;
  phydbl d_max,sum;
  matrix *mat;
  calign *twodata,*tmpdata;
  int state0, state1,len;
  phydbl *F;
  eigen *eigen_struct;


  tmpdata         = (calign *)mCalloc(1,sizeof(calign));
  tmpdata->c_seq  = (align **)mCalloc(2,sizeof(align *));
  tmpdata->b_frq  = (phydbl *)mCalloc(mod->ns,sizeof(phydbl));
  tmpdata->ambigu = (short int *)mCalloc(data->crunch_len,sizeof(short int));
  F               = (phydbl *)mCalloc(mod->ns*mod->ns,sizeof(phydbl ));
  eigen_struct    = (eigen *)Make_Eigen_Struct(mod->ns);

  tmpdata->n_otu  = 2;

  tmpdata->crunch_len = data->crunch_len;
  tmpdata->init_len   = data->init_len;


  mat = NULL;
  if(mod->io->datatype == NT) mat = (mod->whichmodel < 10)?(K80_dist(data,1E+6)):(JC69_Dist(data,mod));
  else if(mod->io->datatype == AA) mat = JC69_Dist(data,mod);
  else if(mod->io->datatype == GENERIC) mat = JC69_Dist(data,mod);
   

  For(i,mod->n_catg) /* Don't use the discrete gamma distribution */
    {
      mod->gamma_rr[i]      = 1.0;
      mod->gamma_r_proba[i] = 1.0;
    }

  n_catg = mod->n_catg;
  mod->n_catg = 1;

  For(j,data->n_otu-1)
    {
      tmpdata->c_seq[0]       = data->c_seq[j];
      tmpdata->c_seq[0]->name = data->c_seq[j]->name;
      tmpdata->wght           = data->wght;

      for(k=j+1;k<data->n_otu;k++)
	{

	  tmpdata->c_seq[1]       = data->c_seq[k];
	  tmpdata->c_seq[1]->name = data->c_seq[k]->name;

	  twodata = Compact_Cdata(tmpdata,mod->io);
	  For(l,mod->ns) twodata->b_frq[l] = data->b_frq[l];
	  Check_Ambiguities(twodata,mod->io->datatype,mod->io->mod->state_len);
	  
	  Hide_Ambiguities(twodata);
	  
	  init = mat->dist[j][k];
	  
	  if((init > DIST_MAX-SMALL) || (init < .0)) init = 0.1;
	  	  
	  d_max = init;
  
	  For(i,mod->ns*mod->ns) F[i]=.0;
	  len = 0;
 	  For(l,twodata->c_seq[0]->len)
	    {
	      state0 = Assign_State(twodata->c_seq[0]->state+l*mod->io->mod->state_len,mod->io->datatype,mod->io->mod->state_len);
	      state1 = Assign_State(twodata->c_seq[1]->state+l*mod->io->mod->state_len,mod->io->datatype,mod->io->mod->state_len);

	      if((state0 > -1) && (state1 > -1))
		{
		  F[mod->ns*state0+state1] += twodata->wght[l];
		  len += (int)twodata->wght[l];
		}
	    }
	  if(len > .0) {For(i,mod->ns*mod->ns) F[i] /= (phydbl)len;}
	  	  
	  sum = 0.;
	  For(i,mod->ns*mod->ns) sum += F[i];
	  	      
	      
	  if(sum < .001) d_max = -1.;
	  else if((sum > 1. - .001) && (sum < 1. + .001)) Opt_Dist_F(&(d_max),F,mod);
	  else
	    {
	      PhyML_Printf("\n. sum = %f\n",sum);
	      PhyML_Printf("\n. Err in file %s at line %d\n\n",__FILE__,__LINE__);
	      Exit("");
	    }
	  	  	  
	  if(d_max >= DIST_MAX) d_max = DIST_MAX;
	  
	  
	  /* Do not correct for dist < BL_MIN, otherwise Fill_Missing_Dist
	   *  will not be called
	   */
	  
	  mat->dist[j][k] = d_max;
	  mat->dist[k][j] = mat->dist[j][k];
	  Free_Cseq(twodata);
	}
    }

  mod->n_catg = n_catg;
    
  Free(tmpdata->ambigu);
  Free(tmpdata->b_frq);
  Free(tmpdata->c_seq);
  free(tmpdata);
  Free_Eigen(eigen_struct);
  Free(F);
  
  return mat;
}

/*********************************************************/

phydbl Lk_Given_Two_Seq(calign *data, int numseq1, int numseq2, phydbl dist, model *mod, phydbl *loglk)
{
  align *seq1,*seq2;
  phydbl site_lk,log_site_lk;
  int i,j,k,l;
/*   phydbl **p_lk_l,**p_lk_r; */
  phydbl *p_lk_l,*p_lk_r;
  phydbl len;
  int dim1,dim2;

  dim1 = mod->ns;
  dim2 = mod->ns * mod->ns;

  DiscreteGamma(mod->gamma_r_proba, mod->gamma_rr, mod->alpha,
		mod->alpha,mod->n_catg,mod->gamma_median);

  seq1 = data->c_seq[numseq1];
  seq2 = data->c_seq[numseq2];


  p_lk_l = (phydbl *)mCalloc(data->c_seq[0]->len * mod->ns,sizeof(phydbl));
  p_lk_r = (phydbl *)mCalloc(data->c_seq[0]->len * mod->ns,sizeof(phydbl));

  if(dist < BL_MIN) dist = BL_START;
  else if(dist > BL_MAX) dist = BL_START;

  For(i,mod->n_catg)
    {
      len = dist*mod->gamma_rr[i];
      if(len < BL_MIN) len = BL_MIN;
      else if(len > BL_MAX) len = BL_MAX;
      PMat(len,mod,dim2*i,mod->Pij_rr);
    }

  if(mod->io->datatype == NT)
    {
      For(i,data->c_seq[0]->len)
	{
	  Init_Tips_At_One_Site_Nucleotides_Float(seq1->state[i],i*mod->ns,p_lk_l);
	  Init_Tips_At_One_Site_Nucleotides_Float(seq2->state[i],i*mod->ns,p_lk_r);
	}
    }
  else if(mod->io->datatype == AA)
    {
      For(i,data->c_seq[0]->len)
	{
	  Init_Tips_At_One_Site_AA_Float(seq1->state[i],i*mod->ns,p_lk_l);
	  Init_Tips_At_One_Site_AA_Float(seq2->state[i],i*mod->ns,p_lk_r);
	}
    }
  else
    {
      PhyML_Printf("\n. Not implemented yet...");
      PhyML_Printf("\n. Err in file %s at line %d\n\n",__FILE__,__LINE__);
      Warn_And_Exit("\n");
    }


  site_lk = .0;
  *loglk = 0;

  For(i,data->c_seq[0]->len)
    {
      if(data->wght[i])
	{
	  site_lk = log_site_lk = .0;
	  if(!data->ambigu[i])
	    {
	      For(k,mod->ns) {if(p_lk_l[i*mod->ns+k] > .0001) break;}
	      For(l,mod->ns) {if(p_lk_r[i*mod->ns+l] > .0001) break;}
	      For(j,mod->n_catg)
		{
		  site_lk +=
		    mod->gamma_r_proba[j] *
		    mod->pi[k] *
		    p_lk_l[i*dim1+k] *
		    mod->Pij_rr[j*dim2+k*dim1+l] *
		    p_lk_r[i*dim1+l];
		}
	    }
	  else
	    {
	      For(j,mod->n_catg)
		{
		  For(k,mod->ns) /*sort sum terms ? No global effect*/
		    {
		      For(l,mod->ns)
			{
			  site_lk +=
			    mod->gamma_r_proba[j] *
			    mod->pi[k] *
			    p_lk_l[i*dim1+k] *
			    mod->Pij_rr[j*dim2+k*dim1+l] *
			    p_lk_r[i*dim1+l];
			}
		    }
		}
	    }

	  if(site_lk <= .0)
	    {
	      PhyML_Printf("'%c' '%c'\n",seq1->state[i],seq2->state[i]);
	      Exit("\n. Err: site lk <= 0\n");
	    }

	  log_site_lk += (phydbl)LOG(site_lk);

	  *loglk += data->wght[i] * log_site_lk;/* sort sum terms ? No global effect*/
	}
    }

/*   For(i,data->c_seq[0]->len) */
/*     { */
/*       Free(p_lk_l[i]); */
/*       Free(p_lk_r[i]); */
/*     } */

  Free(p_lk_l); Free(p_lk_r);
  return *loglk;
}

/*********************************************************/

void Unconstraint_Lk(t_tree *tree)
{
  int i;

  tree->unconstraint_lk = .0;

  For(i,tree->data->crunch_len)
    {
      tree->unconstraint_lk +=
	tree->data->wght[i]*(phydbl)LOG(tree->data->wght[i]);
    }
  tree->unconstraint_lk -=
    tree->data->init_len*(phydbl)LOG(tree->data->init_len);
}

/*********************************************************/

void Make_Tree_4_Lk(t_tree *tree, calign *cdata, int n_site)
{
  int i;

  tree->c_lnL_sorted = (phydbl *)mCalloc(tree->n_pattern, sizeof(phydbl));
  tree->cur_site_lk  = (phydbl *)mCalloc(cdata->crunch_len,sizeof(phydbl));
  tree->old_site_lk  = (phydbl *)mCalloc(cdata->crunch_len,sizeof(phydbl));
  tree->site_lk_cat  = (phydbl *)mCalloc(tree->mod->n_catg,sizeof(phydbl));  
  tree->log_site_lk_cat      = (phydbl **)mCalloc(tree->mod->n_catg,sizeof(phydbl *));
  For(i,tree->mod->n_catg)
    tree->log_site_lk_cat[i] = (phydbl *)mCalloc(cdata->crunch_len,sizeof(phydbl));

  tree->log_lks_aLRT = (phydbl **)mCalloc(3,sizeof(phydbl *));
  For(i,3) tree->log_lks_aLRT[i] = (phydbl *)mCalloc(tree->data->init_len,sizeof(phydbl));

  For(i,2*tree->n_otu-3)
    {
      Make_Edge_Lk(tree->t_edges[i],tree);
      Make_Edge_NNI(tree->t_edges[i]);
    }

  For(i,2*tree->n_otu-2) Make_Node_Lk(tree->noeud[i]);

  if(tree->mod->s_opt->greedy) 
    {
     Init_P_Lk_Tips_Double(tree);
    }
  else 
    {
      Init_P_Lk_Tips_Int(tree);
    }
}

/*********************************************************/

void Init_P_Lk_Tips_Double(t_tree *tree)
{
  int curr_site,i,j,k,dim1,dim2;
  
  dim1 = tree->mod->n_catg * tree->mod->ns;
  dim2 = tree->mod->ns;

  For(curr_site,tree->data->crunch_len)
    {
      For(i,tree->n_otu)
	{
	  if (tree->io->datatype == NT)
	    Init_Tips_At_One_Site_Nucleotides_Float(tree->data->c_seq[i]->state[curr_site],
						    curr_site*dim1+0*dim2,
						    tree->noeud[i]->b[0]->p_lk_rght);

	  else if(tree->io->datatype == AA)
	    Init_Tips_At_One_Site_AA_Float(tree->data->c_seq[i]->state[curr_site],
					   curr_site*dim1+0*dim2,
					   tree->noeud[i]->b[0]->p_lk_rght);

	  else if(tree->io->datatype == GENERIC)
	    Init_Tips_At_One_Site_Generic_Float(tree->data->c_seq[i]->state+curr_site*tree->mod->state_len,
						tree->mod->ns,
						tree->mod->state_len,
						curr_site*dim1+0*dim2,
						tree->noeud[i]->b[0]->p_lk_rght);

	  for(j=1;j<tree->mod->n_catg;j++)
	    {
	      For(k,tree->mod->ns)
		{
		  tree->noeud[i]->b[0]->p_lk_rght[curr_site*dim1+j*dim2+k] = 
		    tree->noeud[i]->b[0]->p_lk_rght[curr_site*dim1+0*dim2+k];
		}
	    }
	}
    }

  #ifdef M4
  if(tree->mod->m4mod) M4_Init_P_Lk_Tips_Double(tree);
  #endif
}

/*********************************************************/

void Init_P_Lk_Tips_Int(t_tree *tree)
{
  int curr_site,i,dim1;

  dim1 = tree->mod->ns;


  For(curr_site,tree->data->crunch_len)
    {
      For(i,tree->n_otu)
	{
	  if(tree->io->datatype == NT)
	    Init_Tips_At_One_Site_Nucleotides_Int(tree->data->c_seq[i]->state[curr_site],
						  curr_site*dim1,
						  tree->noeud[i]->b[0]->p_lk_tip_r);

	  else if(tree->io->datatype == AA)
	    Init_Tips_At_One_Site_AA_Int(tree->data->c_seq[i]->state[curr_site],
					 curr_site*dim1,					   
					 tree->noeud[i]->b[0]->p_lk_tip_r);

	  else if(tree->io->datatype == GENERIC)
	    {
	      Init_Tips_At_One_Site_Generic_Int(tree->data->c_seq[i]->state+curr_site*tree->mod->state_len,
						tree->mod->ns,
						tree->mod->state_len,
						curr_site*dim1,
						tree->noeud[i]->b[0]->p_lk_tip_r);
	    }
	}
    }
 
 #ifdef M4
  if(tree->mod->m4mod) M4_Init_P_Lk_Tips_Int(tree);
  #endif

}

/*********************************************************/

void Update_PMat_At_Given_Edge(t_edge *b_fcus, t_tree *tree)
{
  int i;
  phydbl len;

  len = -1.0;

  if(b_fcus->l < BL_MIN)      b_fcus->l = BL_MIN;
  else if(b_fcus->l > BL_MAX) b_fcus->l = BL_MAX;

  For(i,tree->mod->n_catg)
    {
      if(b_fcus->has_zero_br_len) len = -1.0;
      else
	{
	  len = b_fcus->l*tree->mod->gamma_rr[i];	  
	  if(len < BL_MIN)      len = BL_MIN;
	  else if(len > BL_MAX) len = BL_MAX;
	}
      PMat(len,tree->mod,tree->mod->ns*tree->mod->ns*i,b_fcus->Pij_rr);
    }
}

/*********************************************************/

/* void Update_P_Lk_On_A_Path(t_node *a, t_node *d, t_edge *b, t_node *target_one_side, t_node *target_other_side, t_tree *tree) */
/* { */


/*   /\* */
/*                 \               / */
/* 	   target\___________b_/ */
/* 		 /  \	\  \   \ */
/* 		/    \	 \  \	\ */

/*     target is the root of the subtree at which we want */
/*     the likelihood to be updated */
/*   *\/ */



/* /\*   PhyML_Printf("Update_p_lk on (%d %d) at %d (target=%d %d)\n", *\/ */
/* /\* 	 b->left->num, *\/ */
/* /\* 	 b->rght->num, *\/ */
/* /\* 	 a->num, *\/ */
/* /\* 	 target_one_side->num, *\/ */
/* /\* 	 target_other_side->num); *\/ */

/*   Update_P_Lk(tree,b,a); */
/*   if((a == target_one_side) && (d == target_other_side))  */
/*     return; */
/*   else */
/*     { */
/*       Update_P_Lk_On_A_Path(d, */
/* 			    d->v[tree->t_dir[d->num][target_other_side->num]], */
/* 			    d->b[tree->t_dir[d->num][target_other_side->num]], */
/* 			    target_one_side, */
/* 			    target_other_side, */
/* 			    tree);  */
/*     } */
/* } */

void Update_P_Lk_Along_A_Path(t_node **path, int path_length, t_tree *tree)
{
  int i,j;

  For(i,path_length-1)
    {
      For(j,3)
	if(path[i]->v[j] == path[i+1])
	  {
	    if(path[i] == path[i]->b[j]->left)
	      {
		Update_P_Lk(tree,path[i]->b[j],path[i]->b[j]->left);		    
	      }

	    else if(path[i] == path[i]->b[j]->rght)
	      {
		Update_P_Lk(tree,path[i]->b[j],path[i]->b[j]->rght);
	      }
	    else
	      {
		PhyML_Printf("\n. Err in file %s at line %d\n\n",__FILE__,__LINE__);
		Exit("");
	      }
	    break;
	  }      
#ifdef DEBUG
      if(j == 3)
	{
	  PhyML_Printf("\n. Err in file %s at line %d\n\n",__FILE__,__LINE__);
	  Exit("");
	}
#endif
    }
}

/*********************************************************/

phydbl Lk_Dist(phydbl *F, phydbl dist, model *mod)
{
  int i,j,k;
  phydbl lnL,len;
  int dim1,dim2;

  For(k,mod->n_catg)
    {
      len = dist*mod->gamma_rr[k];
      if(len < BL_MIN)      len = BL_MIN;
      else if(len > BL_MAX) len = BL_MAX;
      PMat(len,mod,mod->ns*mod->ns*k,mod->Pij_rr);
    }
  
  dim1 = mod->ns*mod->ns;
  dim2 = mod->ns;
  lnL = .0;
  For(i,mod->ns)
    {
      For(j,mod->ns)
	{
	  For(k,mod->n_catg)
	    {
 	      lnL += F[dim1*k+dim2*i+j] * LOG(mod->Pij_rr[dim1*k+dim2*i+j]);
	    }
	}
    }

  return lnL;
}

/*********************************************************/

phydbl Update_Lk_At_Given_Edge(t_edge *b_fcus, t_tree *tree)
{
  if(!b_fcus->left->tax) Update_P_Lk(tree,b_fcus,b_fcus->left);
  if(!b_fcus->rght->tax) Update_P_Lk(tree,b_fcus,b_fcus->rght);
  tree->c_lnL = Lk_At_Given_Edge(b_fcus,tree);
  return tree->c_lnL;
}

/*********************************************************/

/*       root
           \
           /
          a
	  |
	  |
	  d
	 / \
        /   \
       w     x    	

       d->t has changed and we need to compute
       the likelihood.
       (1) update the three branch lengths l(ad), l(dw) and l(dx)
       (2) update the change proba matrices along these branches
       (3) update the likelihood of subtree (w,x) (WARNING: (a,x) and (a,w) are not updated)
*/
phydbl Lk_Triplet(t_node *a, t_node *d, t_tree *tree)
{
  int i;
  phydbl max_height;
  phydbl up_bound, low_bound;

  if(d->tax)
    {
      PhyML_Printf("\n. Err in file %s at line %d\n\n",__FILE__,__LINE__);
      Warn_And_Exit("");
    }

  up_bound = low_bound = -1.0;
  max_height = -1.0;
  For(i,3)
    {
      if((d->v[i] != a) && (d->b[i] != tree->e_root))
	{
	  if(tree->rates->nd_t[d->v[i]->num] > max_height)
	    {
	      max_height = tree->rates->nd_t[d->v[i]->num];
	    }
	}
      else
	{
	  up_bound = 
	    (a == tree->n_root)?
	    (tree->rates->nd_t[a->num]):
	    (tree->rates->nd_t[d->v[i]->num]);
	}
    }

  low_bound = max_height;

  if(up_bound < low_bound - 1.E-10)
    {
      PhyML_Printf("\n. a->num=%d d->num=%d",a->num,d->num);
      PhyML_Printf("\n. up_bound = %f, low_bound = %f",up_bound,low_bound);
      Warn_And_Exit("\n");
    }

  if(tree->rates->nd_t[d->num] < low_bound) tree->rates->nd_t[d->num] = low_bound;
  else if(tree->rates->nd_t[d->num] > up_bound) tree->rates->nd_t[d->num] = up_bound;

  /* Step (1) */
  For(i,3)
    {
      if((d->v[i] != a) && (d->b[i] != tree->e_root)) 
	{
	  d->b[i]->l = 
	    (tree->rates->nd_t[d->num] - tree->rates->nd_t[d->v[i]->num]) * 
	    tree->rates->clock_r * 
	    tree->rates->br_r[d->b[i]->num];
	}
      else
	{
	  if(a == tree->n_root)
	    {
	      d->b[i]->l = 
		(tree->rates->nd_t[tree->n_root->num] - tree->rates->nd_t[tree->n_root->v[0]->num] + 
		 tree->rates->nd_t[tree->n_root->num] - tree->rates->nd_t[tree->n_root->v[1]->num]) * tree->rates->clock_r;
	    }
	  else
	    {
	      d->b[i]->l = (tree->rates->nd_t[a->num] - tree->rates->nd_t[d->num]) * tree->rates->clock_r * tree->rates->br_r[d->b[i]->num];	    
	    }
	}
    }
  
  /* Step (2) */
  For(i,3) Update_PMat_At_Given_Edge(d->b[i],tree);
  
  For(i,3) 
    if((d->v[i] == a) || (d->b[i] == tree->e_root))
      {
	Update_P_Lk(tree,d->b[i],d); 
	Lk_At_Given_Edge(d->b[i],tree);
	break;
      }

  return tree->c_lnL;
}

/*********************************************************/

void Print_Lk_Given_Edge_Recurr(t_node *a, t_node *d, t_edge *b, t_tree *tree)
{
  PhyML_Printf("\n___ Edge %3d (left=%3d rght=%3d) lnL=%f",
	 b->num,
	 b->left->num,
	 b->rght->num,
	 Lk_At_Given_Edge(b,tree));

  if(d->tax) return;
  else
    {
      int i;
      For(i,3)
	if(d->v[i] != a)
	  Print_Lk_Given_Edge_Recurr(d,d->v[i],d->b[i],tree);
    }
}

/*********************************************************/

/* Returns a vector containing the posterior probabilities of 
   the different branch rate classes 
*/
phydbl *Post_Prob_Rates_At_Given_Edge(t_edge *b, phydbl *post_prob, t_tree *tree)
{
  phydbl norm_factor;
  int rcat, scale_int;
  phydbl sum,log2,lnL,worst_lnL,best_lnL,mid_lnL;


  For(rcat,tree->mod->n_rr_branch) post_prob[rcat] = .0;

  log2 = 0.6931472;
  
  best_lnL  = UNLIKELY;
  worst_lnL = .0;
  For(rcat,tree->mod->n_rr_branch)
    {
      tree->rates->br_r[b->num] = tree->mod->rr_branch[rcat];
      lnL = Lk_At_Given_Edge(b,tree);

      if(lnL < worst_lnL) worst_lnL = lnL;
      if(lnL > best_lnL)  best_lnL  = lnL;
      post_prob[rcat] = lnL;

      tree->rates->br_r[b->num] = 1.0;
    }

  mid_lnL = worst_lnL + FABS(worst_lnL - best_lnL)/2.;

  /* EXP(LOG(P(D|r))) is hard to compute. Try EXP(LOG(K*P(D|r)))
     instead. The value of K is chosen such that the most accurate
     estimates of the posterior probabilities are obtained for the
     most likely rate classes.
  */
  scale_int = 0;
  do scale_int++;
  while(best_lnL + scale_int * log2 < 20.0);

  norm_factor = .0;
  For(rcat,tree->mod->n_rr_branch)
    {
/*       PhyML_Printf("\n. best_lnL=%f curr_lnL=%f %f %E", */
/*  	     best_lnL, */
/* 	     post_prob[rcat] , */
/* 	     post_prob[rcat] + scale_int * log2, */
/* 	     EXP(post_prob[rcat] + scale_int * log2)); */

      post_prob[rcat] = EXP(post_prob[rcat] + scale_int * log2);


      post_prob[rcat] *= tree->mod->p_rr_branch[rcat];
      norm_factor += post_prob[rcat];
    }

  sum = .0;
  For(rcat,tree->mod->n_rr_branch)
    {
      post_prob[rcat] /= norm_factor;
/*       PhyML_Printf("%f ",post_prob[rcat]); */
      sum += post_prob[rcat];
    }
  
  if(sum < 0.999 || sum > 1.001)
    {
      PhyML_Printf("\n. Err in file %s at line %d\n\n",__FILE__,__LINE__);
      Warn_And_Exit("");
    }

  return post_prob;  
}

/*********************************************************/

phydbl Lk_With_MAP_Branch_Rates(t_tree *tree)
{
  int br,rcat,best_post_prob_cat;
  phydbl *post_prob;
  phydbl best_post_prob;
  t_edge *b;


  post_prob = (phydbl *)mCalloc(tree->mod->n_rr_branch,sizeof(phydbl));
 
  Lk(tree);
  Record_Br_Len(NULL,tree);

  For(br,2*tree->n_otu-3)
    {
      b = tree->t_edges[br];

      /* Compute the posterior probability of each rate class on t_edge b */
      post_prob = (phydbl *)Post_Prob_Rates_At_Given_Edge(b,post_prob,tree);

      /* Find the most probable class */
      best_post_prob = UNLIKELY;
      best_post_prob_cat = -1;
      For(rcat,tree->mod->n_rr_branch)
	{
	  if(post_prob[rcat] > best_post_prob)
	    {
	      best_post_prob = post_prob[rcat];
	      best_post_prob_cat = rcat;
	    }
	}

      /* The relative rate on this branch corresponds to the most probable rate class */
      tree->rates->br_r[br] = tree->mod->rr_branch[best_post_prob_cat];
    }

  Lk(tree);
  
  For(br,2*tree->n_otu-3) tree->rates->br_r[br] = 1.0;

  Free(post_prob);

  return tree->c_lnL;
}

/*********************************************************/
/*********************************************************/
/*********************************************************/
/*********************************************************/
/*********************************************************/
/*********************************************************/
/*********************************************************/
/*********************************************************/
/*********************************************************/

/* Core of the likelihood calculcation. Assume that the partial likelihoods on both
   sides of t_edge *b are up-to-date. Calculate the log-likelihood at one site.
*/
#ifdef USE_OLD_LK
phydbl Lk_Core(t_edge *b, t_tree *tree)
{
  phydbl log_site_lk, site_lk, site_lk_cat;
  phydbl scale_left, scale_rght;
  phydbl sum;
  int ambiguity_check,state;
  int catg,ns,k,l,site;
  int dim1,dim2,dim3;


  dim1 = tree->mod->n_catg * tree->mod->ns;
  dim2 = tree->mod->ns;
  dim3 = tree->mod->ns * tree->mod->ns;

  log_site_lk     = .0;
  site_lk         = .0;
  site_lk_cat     = .0;
  ambiguity_check = -1;
  state           = -1;
  site            = tree->curr_site;
  ns              = tree->mod->ns;

  tree->old_site_lk[site] = tree->cur_site_lk[site];

  /* Get the vectors of partial likelihood scaling */
  scale_left =
    (b->sum_scale_left)?
    (b->sum_scale_left[site]):
    (0.0);
  
  scale_rght =
    (b->sum_scale_rght)?
    (b->sum_scale_rght[site]):
    (0.0);

  if((b->rght->tax) && (!tree->mod->s_opt->greedy))
    {
      ambiguity_check = tree->data->c_seq[b->rght->num]->is_ambigu[site];
      if(!ambiguity_check) state = Get_State_From_P_Pars(b->p_lk_tip_r,site*dim2,tree);
    }

  if(tree->mod->use_m4mod) ambiguity_check = 1;
  
  /* For all classes of rates */
  For(catg,tree->mod->n_catg)
    {
      site_lk_cat = .0;

      /* b is an external edge */
      if((b->rght->tax) && (!tree->mod->s_opt->greedy))
        {
          /* If the character observed at the tip is NOT ambiguous: ns x 1 terms to consider */
          if(!ambiguity_check)
            {
              sum = .0;
              For(l,ns)
                {
                  sum +=
                    b->Pij_rr[catg*dim3+state*dim2+l] *
                    (phydbl)b->p_lk_left[site*dim1+catg*dim2+l];
                }
              site_lk_cat += sum * tree->mod->pi[state];
            }
          /* If the character observed at the tip is ambiguous: ns x ns terms to consider */
          else
            {
              For(k,ns)
                {
                  sum = .0;
                  if(b->p_lk_tip_r[site*dim2+k] > .0)
                    {
                      For(l,ns)
                        {
                          sum +=
                            b->Pij_rr[catg*dim3+k*dim2+l] *
                            (phydbl)b->p_lk_left[site*dim1+catg*dim2+l];
                        }
                      site_lk_cat +=
                        sum *
                        tree->mod->pi[k] *
                        (phydbl)b->p_lk_tip_r[site*dim2+k];
                    }
                }
            }
        }
      /* b is an internal edge: ns x ns terms to consider */
      else
        {
          For(k,ns)
            {
              sum = .0;
              if(b->p_lk_rght[site*dim1+catg*dim2+k] > .0)
                {
                  For(l,ns)
                    {
                      sum +=
                        b->Pij_rr[catg*dim3+k*dim2+l] *
                        (phydbl)b->p_lk_left[site*dim1+catg*dim2+l];
                    }
                  site_lk_cat +=
                    sum *
                    tree->mod->pi[k] *
                    (phydbl)b->p_lk_rght[site*dim1+catg*dim2+k];
                }
            }
        }

      tree->log_site_lk_cat[catg][site] = site_lk_cat;
      site_lk += site_lk_cat * tree->mod->gamma_r_proba[catg];
    }

  /* site_lk may be too small ? */
  if(site_lk < 1.E-300) site_lk = 1.E-300;

  /* The substitution model does not consider invariable sites */
  if(!tree->mod->invar)
    {
      log_site_lk = (phydbl)LOG(site_lk) + (phydbl)scale_left + (phydbl)scale_rght;
    }
  else
    {
      /* The site is invariant */
      if((phydbl)tree->data->invar[site] > -0.5)
        {
          if((scale_left + scale_rght > 0.0) || (scale_left + scale_rght < 0.0))
            site_lk *= (phydbl)EXP(scale_left + scale_rght);
          
          log_site_lk =
            (phydbl)LOG(site_lk*(1.0-tree->mod->pinvar) + tree->mod->pinvar*tree->mod->pi[tree->data->invar[site]]);
          /* LOG(P(D)) = LOG(P(D | subst. rate > 0) * P(subst. rate > 0) + P(D | subst. rate = 0) * P(subst. rate = 0)) */
        }
      else
        {
          log_site_lk = (phydbl)LOG(site_lk*(1.0-tree->mod->pinvar)) + (phydbl)scale_left + (phydbl)scale_rght;
          /* Same formula as above with P(D | subs, rate = 0) = 0 */
        }
    }
  
  if(log_site_lk < -BIG) Warn_And_Exit("\nlog_site_lk < -BIG\n");

  For(catg,tree->mod->n_catg)
    tree->log_site_lk_cat[catg][site] =
    LOG(tree->log_site_lk_cat[catg][site]) +
    scale_left +
    scale_rght;
  
  tree->cur_site_lk[site] = log_site_lk;
  /* Multiply log likelihood by the number of times this site pattern is found oin the data */
  tree->c_lnL_sorted[site] = tree->data->wght[site]*log_site_lk;
  tree->c_lnL += tree->c_lnL_sorted[site];
  return log_site_lk;
}

/*********************************************************/

/* Update partial likelihood on edge b on the side of b where
   node d lies.
*/
void Update_P_Lk(t_tree *tree, t_edge *b, t_node *d)
{
/*
           |
           |<- b
           |
           d
          / \
         /   \
        /     \
        n_v1   n_v2
*/
  t_node *n_v1, *n_v2;
  phydbl p1_lk1,p2_lk2;
  phydbl *p_lk,*p_lk_v1,*p_lk_v2;
  phydbl *Pij1,*Pij2;
  phydbl max_p_lk;
  phydbl *sum_scale;
  phydbl scale_v1, scale_v2;
  int i,j;
  int catg,site;
  int dir1,dir2;
  int n_patterns;
  int ambiguity_check_v1,ambiguity_check_v2;
  int state_v1,state_v2;
  int dim1, dim2, dim3;

  dim1 = tree->mod->n_catg * tree->mod->ns;
  dim2 = tree->mod->ns;
  dim3 = tree->mod->ns * tree->mod->ns;

  state_v1 = state_v2 = -1;
  ambiguity_check_v1 = ambiguity_check_v2 = -1;
  scale_v1 = scale_v2 = 0.0;
  p1_lk1 = p2_lk2 = .0;


  if(d->tax)
    {
      PhyML_Printf("\n. t_node %d is a leaf...",d->num);
      PhyML_Printf("\n. Err in file %s at line %d\n\n",__FILE__,__LINE__);
      Warn_And_Exit("\n");
    }

  n_patterns = tree->n_pattern;
  
  /* TO DO: Might be worth keeping these directions in memory instead of
     calculating them every time... */
  dir1=dir2=-1;
  For(i,3) if(d->b[i] != b) (dir1<0)?(dir1=i):(dir2=i);

  if((dir1 == -1) || (dir2 == -1))
    {
      PhyML_Printf("\n. d = %d",d->num);
      PhyML_Printf("\n. d->v[0] = %d, d->v[1] = %d, d->v[2] = %d",d->v[0]->num,d->v[1]->num,d->v[2]->num);
      PhyML_Printf("\n. d->b[0] = %d, d->b[1] = %d, d->b[2] = %d",d->b[0]->num,d->b[1]->num,d->b[2]->num);
      PhyML_Printf("\n. d->num = %d dir1 = %d dir2 = %d",d->num,dir1,dir2);
      PhyML_Printf("\n. Err in file %s at line %d\n\n",__FILE__,__LINE__);
      Exit("");
    }
  
  n_v1 = d->v[dir1];
  n_v2 = d->v[dir2];

  /* Get the partial likelihood vectors on edge b and the two pendant
     edges (i.e., the two other edges connected to d) */
  if(d == b->left)
    {
      p_lk = b->p_lk_left;
      sum_scale = b->sum_scale_left;
    }
  else
    {
      p_lk = b->p_lk_rght;
      sum_scale = b->sum_scale_rght;
    }
      
  if(d == d->b[dir1]->left)
    {
      p_lk_v1 = d->b[dir1]->p_lk_rght;
    }
  else
    {
      p_lk_v1 = d->b[dir1]->p_lk_left;
    }
  
  if(d == d->b[dir2]->left)
    {
      p_lk_v2 = d->b[dir2]->p_lk_rght;
    }
  else
    {
      p_lk_v2 = d->b[dir2]->p_lk_left;
    }
  
  /* Change probability matrices on the two pendant edges */
  Pij1 = d->b[dir1]->Pij_rr;
  Pij2 = d->b[dir2]->Pij_rr;
  
  /* For every site in the alignment */
  For(site,n_patterns)
    {
      /* Current scaling values at that site */
/*       scale_v1 = (sum_scale_v1)?(sum_scale_v1[site]):(0.0); */
/*       scale_v2 = (sum_scale_v2)?(sum_scale_v2[site]):(0.0);  */
/*       sum_scale[site] = scale_v1 + scale_v2; */

      max_p_lk = -BIG;
      state_v1 = state_v2 = -1;
      ambiguity_check_v1 = ambiguity_check_v2 = -1;
      
      if(!tree->mod->s_opt->greedy)
        {
          /* n_v1 and n_v2 are tip nodes */
          if(n_v1->tax)
            {
              /* Is the state at this tip ambiguous? */
              ambiguity_check_v1 = tree->data->c_seq[n_v1->num]->is_ambigu[site];
              if(!ambiguity_check_v1) state_v1 = Get_State_From_P_Pars(n_v1->b[0]->p_lk_tip_r,site*dim2,tree);
            }
              
          if(n_v2->tax)
            {
              /* Is the state at this tip ambiguous? */
              ambiguity_check_v2 = tree->data->c_seq[n_v2->num]->is_ambigu[site];
              if(!ambiguity_check_v2) state_v2 = Get_State_From_P_Pars(n_v2->b[0]->p_lk_tip_r,site*dim2,tree);
            }
        }
      
      if(tree->mod->use_m4mod)
        {
          ambiguity_check_v1 = 1;
          ambiguity_check_v2 = 1;
        }

      /* For all the rate classes */
      For(catg,tree->mod->n_catg)
        {
          /* For all the state at node d */
          For(i,tree->mod->ns)
            {
              p1_lk1 = .0;
              
              /* n_v1 is a tip */
              if((n_v1->tax) && (!tree->mod->s_opt->greedy))
                {
                  if(!ambiguity_check_v1)
                    {
                      /* For the (non-ambiguous) state at node n_v1 */
                      p1_lk1 = Pij1[catg*dim3+i*dim2+state_v1];
                    }
                  else
                    {
                      /* For all the states at node n_v1 */
                      For(j,tree->mod->ns)
                        {
                          p1_lk1 += Pij1[catg*dim3+i*dim2+j] * (phydbl)n_v1->b[0]->p_lk_tip_r[site*dim2+j];
                        }
                    }
                }
              /* n_v1 is an internal node */
              else
                {
                  /* For the states at node n_v1 */
                  For(j,tree->mod->ns)
                    {
                      p1_lk1 += Pij1[catg*dim3+i*dim2+j] * (phydbl)p_lk_v1[site*dim1+catg*dim2+j];
                    }
                }
              
              p2_lk2 = .0;
              
              /* We do exactly the same as for node n_v1 but for node n_v2 this time.*/
              /* n_v2 is a tip */
              if((n_v2->tax) && (!tree->mod->s_opt->greedy))
                {
                  if(!ambiguity_check_v2)
                    {
                      /* For the (non-ambiguous) state at node n_v2 */
                      p2_lk2 = Pij2[catg*dim3+i*dim2+state_v2];
                    }
                  else
                    {
                      /* For all the states at node n_v2 */
                      For(j,tree->mod->ns)
                        {
                          p2_lk2 += Pij2[catg*dim3+i*dim2+j] * (phydbl)n_v2->b[0]->p_lk_tip_r[site*dim2+j];
                        }
                    }
                }
              /* n_v2 is an internal node */
              else
                {
                  /* For all the states at node n_v2 */
                  For(j,tree->mod->ns)
                    {
                      p2_lk2 += Pij2[catg*dim3+i*dim2+j] * (phydbl)p_lk_v2[site*dim1+catg*dim2+j];
                    }
                }
              
              p_lk[site*dim1+catg*dim2+i] = (phydbl)(p1_lk1 * p2_lk2);
              
              /* Work out the maximum value of the partial likelihoods at node d */
              if(p_lk[site*dim1+catg*dim2+i] > max_p_lk) max_p_lk = p_lk[site*dim1+catg*dim2+i];
            }
        }
      
      /* Scaling */
      if((max_p_lk < LIM_SCALE_VAL) || (max_p_lk > (1./LIM_SCALE_VAL)))
        {
          /* For each rate class */
          For(catg,tree->mod->n_catg)
            {
              /* For each state at node d */
              For(i,tree->mod->ns)
                {
                  /* Divide the corresponding partial likelihood by the maximum
                     value of the partial likelihoods calculated over all rate
                     classes and states. */
                  p_lk[site*dim1+catg*dim2+i] /= max_p_lk;
                  
/*                if((p_lk[site][catg][i] > BIG) || (p_lk[site][catg][i] < SMALL)) */
/*                  { */
/*                    PhyML_Printf("\n. Err in file %s at line %d\n\n",__FILE__,__LINE__); */
/*                    PhyML_Printf("\n. p_lk[%3d][%2d][%3d] = %G max_p_lk = %G",site,catg,i,p_lk[site][catg][i],max_p_lk); */
/*                    PhyML_Printf("\n. alpha=%f pinv=%f",tree->mod->alpha,tree->mod->pinvar); */
/*                    For(i,tree->mod->n_catg) PhyML_Printf("\n. rr[%2d] = %G",i,tree->mod->rr[i]); */
/*                    PhyML_Printf("\n. d->b[dir1]->l = %f, d->b[dir2]->l = %f",d->b[dir1]->l,d->b[dir2]->l); */
/*                    PhyML_Printf("\n. d->v[dir1]->num = %d, d->v[dir2]->num = %d",d->v[dir1]->num,d->v[dir2]->num); */
/*                    if(d->v[dir1]->tax) */
/*                      { */
/*                        PhyML_Printf("\n. Character observed at d->v[dir1] = %d",state_v1); */
/*                      } */
/*                    if(d->v[dir2]->tax) */
/*                      { */
/*                        PhyML_Printf("\n. Character observed at d->v[dir2] = %d",state_v2); */
/*                      } */
/*                    Warn_And_Exit("\n. Numerical precision problem ! (send me an e-mail : s.guindon@auckland.ac.nz)\n"); */
/*                  } */
                }
            }
          sum_scale[site] += (phydbl)LOG(max_p_lk);
        }
    }
}

#endif 
/*********************************************************/

