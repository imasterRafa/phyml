/*

PhyML:  a program that  computes maximum likelihood phyLOGenies from
DNA or AA homoLOGous sequences.

Copyright (C) Stephane Guindon. Oct 2003 onward.

All parts of the source except where indicated are distributed under
the GNU public licence. See http://www.opensource.org for details.

*/



#include "mcmc.h"

/*********************************************************/

void MCMC(t_tree *tree)
{
  int move;
  phydbl u;
  int change;
  int first,secod;
  int i;


  if(tree->mcmc->randomize)
    {
      MCMC_Randomize_Nu(tree);
      MCMC_Randomize_Node_Times(tree);
      MCMC_Sim_Rate(tree->n_root,tree->n_root->v[0],tree);
      MCMC_Sim_Rate(tree->n_root,tree->n_root->v[1],tree);
      MCMC_Randomize_Node_Rates(tree);
      MCMC_Randomize_Clock_Rate(tree); /* Clock Rate must be the last parameter to be randomized */
    }

  tree->mod->update_eigen = YES;
  
  MCMC_Initialize_Param_Val(tree->mcmc,tree);
  MCMC_Print_Param(tree->mcmc,tree);

  Update_Ancestors(tree->n_root,tree->n_root->v[0],tree);
  Update_Ancestors(tree->n_root,tree->n_root->v[1],tree);
  
  For(i,2*tree->n_otu-2) tree->rates->br_do_updt[i] = YES;
  RATES_Update_Cur_Bl(tree);
  RATES_Lk_Rates(tree);
  tree->both_sides = NO;
  if(tree->mcmc->use_data) Lk(tree);
  else tree->c_lnL = 0.0;

  tree->mod->update_eigen = NO;

  first = 0;
  secod = 1;
  do
    {

      /* if(tree->mcmc->run > 10000) */
      /* 	{ */
      /* 	  FILE *fp; */
      /* 	  fp = fopen("simul_seq","w"); */
      /* 	  MCMC_Print_Param(tree->mcmc,tree); */
      /* 	  Evolve(tree->data,tree->mod,tree); */
      /* 	  Print_CSeq(fp,tree->data); */
      /* 	  fflush(NULL); */
      /* 	  fclose(fp); */
      /* 	  Exit("\n"); */
      /* 	} */

      u = Uni();

      For(move,tree->mcmc->n_moves) if(tree->mcmc->move_weight[move] > u) break;
      
      if(u < .5) { first = 0; secod = 1; }
      else       { first = 1; secod = 0; }


      /* PhyML_Printf("\n. %18s %12f",tree->mcmc->move_name[move],tree->rates->c_lnL); */

      
      /* Clock rate */
      if(!strcmp(tree->mcmc->move_name[move],"clock"))
      	{
      	  For(i,2*tree->n_otu-2) tree->rates->br_do_updt[i] = YES;
	  
      	  MCMC_Single_Param_Generic(&(tree->rates->clock_r),tree->rates->min_clock,tree->rates->max_clock,move,
      	  			    NULL,&(tree->c_lnL),
      	  			    NULL,Wrap_Lk,MCMC_MOVE_RANDWALK,NO,NULL,tree,NULL);
      	}

      /* Nu */
      else if(!strcmp(tree->mcmc->move_name[move],"nu"))
      	{
      	  For(i,2*tree->n_otu-2) tree->rates->br_do_updt[i] = YES;
	  MCMC_Nu(tree);
      	}

      /* Tree height */
      else if(!strcmp(tree->mcmc->move_name[move],"tree_height"))
      	{
      	  MCMC_Tree_Height(tree);
      	}

      /* Subtree height */
      else if(!strcmp(tree->mcmc->move_name[move],"subtree_height"))
      	{
      	  MCMC_Subtree_Height(tree);
      	}

      /* Subtree rates */
      else if(!strcmp(tree->mcmc->move_name[move],"subtree_rates"))
      	{
      	  MCMC_Subtree_Rates(tree);
      	}

      /* Swing rates */
      else if(!strcmp(tree->mcmc->move_name[move],"tree_rates"))
      	{
      	  MCMC_Tree_Rates(tree);
      	}

      else if(!strcmp(tree->mcmc->move_name[move],"updown_nu_cr"))
      	{
      	  MCMC_Updown_Nu_Cr(tree);
      	}


      /* Ts/tv ratio */
      else if(!strcmp(tree->mcmc->move_name[move],"kappa"))
      	{
      	  change = NO;
      	  tree->mod->update_eigen = YES;
		
      	  if(tree->io->lk_approx == NORMAL)
      	    {
      	      tree->io->lk_approx = EXACT;
      	      if(tree->mcmc->use_data == YES) Lk(tree);
      	      change = YES;
      	    }
	  
      	  For(i,2*tree->n_otu-2) tree->rates->br_do_updt[i] = NO;
      	  MCMC_Single_Param_Generic(&(tree->mod->kappa),0.0,20.,move,
      	  			    NULL,&(tree->c_lnL),
      	  			    NULL,Wrap_Lk,MCMC_MOVE_SCALE,NO,NULL,tree,NULL);
	  
      	  if(change == YES)
      	    {
      	      tree->io->lk_approx = NORMAL;
      	      if(tree->mcmc->use_data == YES) Lk(tree);
      	    }
	  
      	  tree->mod->update_eigen = NO;
      	}

      /* Times */
      else if(!strcmp(tree->mcmc->move_name[move],"time"))
      	{
      	  tree->both_sides = YES;
      	  if(tree->mcmc->use_data == YES) Lk(tree);
      	  tree->both_sides = NO;

      	  if(tree->mcmc->is == NO || tree->rates->model_log_rates == YES)
      	    {
	      MCMC_One_Time(tree->n_root,tree->n_root->v[first],YES,tree);
	      MCMC_One_Time(tree->n_root,tree->n_root->v[secod],YES,tree);
      	    }
      	  else
      	    {
	      /* MCMC_One_Time(tree->n_root,tree->n_root->v[first],YES,tree); */
	      /* MCMC_One_Time(tree->n_root,tree->n_root->v[secod],YES,tree); */
      	      RATES_Posterior_One_Time(tree->n_root,tree->n_root->v[first],YES,tree);
      	      RATES_Posterior_One_Time(tree->n_root,tree->n_root->v[secod],YES,tree);
      	    }
      	}
      
      /* Node Rates */
      else if(!strcmp(tree->mcmc->move_name[move],"nd_rate"))
      	{
      	  MCMC_One_Node_Rate(tree->n_root,tree->n_root->v[first],YES,tree);
      	  MCMC_One_Node_Rate(tree->n_root,tree->n_root->v[secod],YES,tree);
      	}

      /* Edge Rates */
      else if(!strcmp(tree->mcmc->move_name[move],"br_rate"))
      	{

      	  tree->both_sides = YES;
      	  if(tree->mcmc->use_data == YES) Lk(tree);
      	  tree->both_sides = NO;


      	  if(tree->mcmc->is == NO)
      	    {
      	      /* MCMC_Slice_One_Rate(tree->n_root,tree->n_root->v[first],YES,tree); */
      	      /* MCMC_Slice_One_Rate(tree->n_root,tree->n_root->v[secod],YES,tree); */

	      MCMC_One_Rate(tree->n_root,tree->n_root->v[first],YES,tree);
	      MCMC_One_Rate(tree->n_root,tree->n_root->v[secod],YES,tree);
      	    }
      	  else
      	    {
      	      RATES_Posterior_One_Rate(tree->n_root,tree->n_root->v[first],YES,tree);
      	      RATES_Posterior_One_Rate(tree->n_root,tree->n_root->v[secod],YES,tree);
      	    }

	  /* MCMC_Sim_Rate(tree->n_root,tree->n_root->v[0],tree); */
	  /* MCMC_Sim_Rate(tree->n_root,tree->n_root->v[1],tree); */

      	}

      tree->mcmc->run++;
      MCMC_Get_Acc_Rates(tree->mcmc);

      /* !!!!!!!!!!!!!!!!1 */
      /* Is it necessary? */
      /* RATES_Lk_Rates(tree); */
      /* if(tree->mcmc->use_data == YES) Lk(tree); */
      
      /* PhyML_Printf("%12f ",tree->rates->c_lnL); */

      MCMC_Print_Param(tree->mcmc,tree);
      MCMC_Print_Param_Stdin(tree->mcmc,tree);


      (void)signal(SIGINT,MCMC_Terminate); 
    }
  while(tree->mcmc->run < tree->mcmc->chain_len);
}

/*********************************************************/

void MCMC_Single_Param_Generic(phydbl *val, 
			       phydbl lim_inf, 
			       phydbl lim_sup, 
			       int move_num,
			       phydbl *lnPrior,
			       phydbl *lnLike,
			       phydbl (*prior_func)(t_edge *,t_tree *,supert_tree *), 
			       phydbl (*like_func)(t_edge *,t_tree *,supert_tree *),
			       int move_type,
			       int _log, /* _log == YES: the model describes the distribution of log(val) but the move applies to val. Need a correction factor */
			       t_edge *branch, t_tree *tree, supert_tree *stree)
{
  phydbl cur_val,new_val,new_lnLike,new_lnPrior,cur_lnLike,cur_lnPrior;
  phydbl u,alpha,ratio;
  phydbl K,mult;
  phydbl new_lnval, cur_lnval;
 
  Record_Br_Len(tree);

  cur_val       = *val;
  new_val       = -1.0;
  ratio         =  0.0;
  mult          = -1.0;
  K             = tree->mcmc->tune_move[move_num];
  cur_lnval     = LOG(*val);
  new_lnval     = cur_lnval;

  if(lnLike)
    {
      cur_lnLike = *lnLike;
      new_lnLike = *lnLike;
    }
  else
    {
      cur_lnLike = 0.0;
      new_lnLike = 0.0;
    }
  
  if(lnPrior)
    {
      cur_lnPrior = *lnPrior;
      new_lnPrior = *lnPrior;
    }
  else
    {
      cur_lnPrior = 0.0;
      new_lnPrior = 0.0;
    }

  u = Uni();

  if(move_type == MCMC_MOVE_SCALE)
    {
      /* Thorne's move. log(HR) = + log(mult) */
        mult    = EXP(K*(u-0.5));
        new_val = cur_val * mult;
    }
  else if(move_type == MCMC_MOVE_LOG_RANDWALK)
    {
      /* Random walk on the log scale. Watch out for the change of variable. */
      new_lnval = u * (2.*K) + cur_lnval - K;
      new_val   = EXP(new_lnval);
    }
  else if(move_type == MCMC_MOVE_RANDWALK)
    {
      /* Random walk on the original scale. log(HR) = 0 */
      new_val = u*(2.*K) + cur_val - K;
      new_val = Reflect(new_val,lim_inf,lim_sup);
    }


  if(new_val < lim_sup && new_val > lim_inf)
    {

      *val = new_val;
  
      RATES_Update_Cur_Bl(tree);
      
      ratio = 0.0;
      
      if(move_type == MCMC_MOVE_LOG_RANDWALK) ratio += (new_lnval - cur_lnval); /* Change of variable */
      if(move_type == MCMC_MOVE_SCALE)        ratio += LOG(mult);               /* Hastings ratio */
      if(move_type == MCMC_MOVE_RANDWALK)     ratio += .0;                      /* Hastings ratio */ 

      if(_log == YES) ratio += (cur_lnval - new_lnval);
      
      if(prior_func) /* Prior ratio */
	{ 
	  new_lnPrior = (*prior_func)(branch,tree,stree); 
	  ratio += (new_lnPrior - cur_lnPrior); 
	}
      
      if(like_func && tree->mcmc->use_data == YES)  /* Likelihood ratio */
	{ 
	  new_lnLike  = (*like_func)(branch,tree,stree);  
	  ratio += (new_lnLike - cur_lnLike);  
	}
      

      ratio = EXP(ratio);
      alpha = MIN(1.,ratio);


      u = Uni();
      if(u > alpha) /* Reject */
	{
	  *val    = cur_val;
	  new_val = cur_val;
	  if(lnPrior) *lnPrior = cur_lnPrior;
	  if(lnLike)  *lnLike  = cur_lnLike;
	  Restore_Br_Len(tree);
	}
      else /* Accept */
	{
	  /* PhyML_Printf("\n. diff = %f move=%s priors=%f %f",new_lnLike-cur_lnLike,tree->mcmc->move_name[move_num],new_lnPrior,cur_lnPrior); */	  
	  tree->mcmc->acc_move[move_num]++;
	  if(lnPrior) *lnPrior = new_lnPrior;
	  if(lnLike)  *lnLike  = new_lnLike;
	}      
    }
      
  tree->mcmc->run_move[move_num]++;
}

/*********************************************************/

void MCMC_Update_Effective_Sample_Size(int move_num, t_mcmc *mcmc, t_tree *tree)
{
  phydbl new_val,cur_val;

  mcmc->ess_run[move_num]++;

  new_val = mcmc->new_param_val[move_num];
  cur_val = mcmc->old_param_val[move_num];

  if(mcmc->ess_run[move_num] == 1)
    {
      mcmc->first_val[move_num] = cur_val;
      mcmc->sum_val[move_num]   = cur_val;
      mcmc->sum_valsq[move_num] = POW(cur_val,2);
      return;
    }

  mcmc->sum_val[move_num]            += new_val;
  mcmc->sum_valsq[move_num]          += POW(new_val,2);
  mcmc->sum_curval_nextval[move_num] += cur_val * new_val;
  	
  mcmc->ess[move_num] = 
    Effective_Sample_Size(mcmc->first_val[move_num],
			  new_val,
			  mcmc->sum_val[move_num],
			  mcmc->sum_valsq[move_num],
			  mcmc->sum_curval_nextval[move_num],
			  mcmc->ess_run[move_num]);
	
  mcmc->old_param_val[move_num] = new_val;

  if(move_num == mcmc->num_move_nd_t+tree->n_root->num-tree->n_otu)
    {
      /* FILE *fp; */
      /* fp = fopen("out","a"); */
      /* fprintf(fp,"%f\n",new_val); */
      /* fclose(fp); */
      /* printf("\n. first=%G last=%G sum=%f sum_valsq=%f sum_cur_next=%f run=%d ess=%f", */
      /* 	     mcmc->first_val[move_num],new_val, */
      /* 	     mcmc->sum_val[move_num], */
      /* 	     mcmc->sum_valsq[move_num], */
      /* 	     mcmc->sum_curval_nextval[move_num], */
      /* 	     mcmc->run_move[move_num]+1, */
      /* 	     mcmc->ess[move_num] */
      /* 	     ); */
    }
      
}

/*********************************************************/

void MCMC_Sample_Joint_Rates_Prior(t_tree *tree)
{
  int i,dim;
  phydbl T;
  phydbl *r,*t,*lambda;
  phydbl *min_r,*max_r;
  phydbl k;

  dim    = 2*tree->n_otu-2;
  lambda = tree->rates->_2n_vect1;
  min_r  = tree->rates->_2n_vect2;
  max_r  = tree->rates->_2n_vect3;
  r      = tree->rates->br_r;
  t      = tree->rates->nd_t;

  For(i,dim) tree->rates->mean_r[i] = 1.0;

  RATES_Fill_Lca_Table(tree);
  RATES_Covariance_Mu(tree);

  T = .0;
  For(i,dim) T += (t[tree->noeud[i]->num] - t[tree->noeud[i]->anc->num]);
  For(i,dim) lambda[i] = (t[tree->noeud[i]->num] - t[tree->noeud[i]->anc->num])/T;
  For(i,dim) r[i] = 1.0;
  For(i,dim) min_r[i] = tree->rates->min_rate;
  For(i,dim) max_r[i] = tree->rates->max_rate;

  k = 1.; /* We want \sum_i lambda[i] r[i] = 1 */

  Rnorm_Multid_Trunc_Constraint(tree->rates->mean_r, 
				tree->rates->cov_r, 
				min_r,max_r, 
				lambda,
				k, 
				r,
				dim);
}
  
/*********************************************************/

void MCMC_One_Rate(t_node *a, t_node *d, int traversal, t_tree *tree)
{
  t_edge *b;
  int i;
  phydbl u;
  phydbl new_lnL_data, cur_lnL_data, new_lnL_rate, cur_lnL_rate;
  phydbl ratio, alpha;
  phydbl new_mu, cur_mu;
  phydbl r_min, r_max;
  phydbl K;
  t_edge *b1,*b2,*b3;
  t_node *v2,*v3;
  phydbl nu,cr;
  
  b = NULL;
  if(a == tree->n_root) b = tree->e_root;
  else
    For(i,3) if(d->v[i] == a) { b = d->b[i]; break; }
  
  
  
  cur_mu       = tree->rates->br_r[d->num];
  cur_lnL_data = tree->c_lnL;
  new_lnL_data = tree->c_lnL;
  cur_lnL_rate = tree->rates->c_lnL;
  new_lnL_rate = tree->rates->c_lnL;
  r_min        = tree->rates->min_rate;
  r_max        = tree->rates->max_rate;
  K            = tree->mcmc->tune_move[tree->mcmc->num_move_br_r+d->num];
  ratio        = 0.0;
  nu           = tree->rates->nu;
  cr           = tree->rates->clock_r;
  
  Record_Br_Len(tree);
  
  u = Uni();
  
  if(tree->rates->model_log_rates == YES)
    {
      /* new_mu = u*(2.*K) + cur_mu - K; */
      cur_mu = EXP(cur_mu);
      new_mu = cur_mu * EXP(K*(u-0.5));
      r_min  = EXP(r_min);
      r_max  = EXP(r_max);
    }
  else
    {	  
      new_mu = Rnorm(cur_mu,K);
      new_mu = Reflect(new_mu,r_min,r_max);
    }
  
  if(new_mu > r_min && new_mu < r_max)
    {
      
      if(tree->rates->model_log_rates == YES)
	tree->rates->br_r[d->num] = LOG(new_mu);
      else
	tree->rates->br_r[d->num] = new_mu;
      
      
      v2 = v3 = NULL;
      For(i,3)
	if((d->v[i] != a) && (d->b[i] != tree->e_root))
	  {
	    if(!v2) { v2 = d->v[i]; }
	    else    { v3 = d->v[i]; }
	  }
      
      
      b1 = NULL;
      if(a == tree->n_root) b1 = tree->e_root;
      else For(i,3) if(d->v[i] == a) { b1 = d->b[i]; break; }
      
      b2 = b3 = NULL;
      if(!d->tax)
	{
	  For(i,3)
	    if((d->v[i] != a) && (d->b[i] != tree->e_root))
	      {
		if(!b2) { b2 = d->b[i]; }
		else    { b3 = d->b[i]; }
	      }
	}
      
      tree->rates->br_do_updt[d->num] = YES;
      if(!d->tax)
	{
	  tree->rates->br_do_updt[v2->num] = YES;
	  tree->rates->br_do_updt[v3->num] = YES;
	}
      
      RATES_Update_Cur_Bl(tree);
      
      /* printf("\n. r0=%f r1=%f cr=%f mean=%f var=%f nu=%f dt=%f", */
      /* 	 r0,r1,tree->rates->clock_r,b1->gamma_prior_mean,b1->gamma_prior_var,nu,t1-t0); */
      
      
      if(tree->mcmc->use_data)
	{
	  if(tree->io->lk_approx == EXACT)
	    {
	      Update_PMat_At_Given_Edge(b1,tree);
	      if(!d->tax)
		{
		  Update_PMat_At_Given_Edge(b2,tree);
		  Update_PMat_At_Given_Edge(b3,tree);
		}
	      Update_P_Lk(tree,b1,d);
	    }
	  new_lnL_data = Lk_At_Given_Edge(b1,tree);
	  
	  /* tree->both_sides = NO; */
	  /* new_lnL_data = Lk(tree); */
	}
      
      new_lnL_rate = RATES_Lk_Rates(tree);
      
      /* if(tree->rates->model_log_rates == YES) ratio += 0.0; */
      if(tree->rates->model_log_rates == YES) ratio += LOG(new_mu/cur_mu);
      /* else                                    ratio += (-1.)*LOG(new_mu/cur_mu); */
      
      ratio += (new_lnL_rate - cur_lnL_rate);
      
      if(tree->mcmc->use_data) ratio += (new_lnL_data - cur_lnL_data);
      
      /* If modelling log or rates instead of rates */
      if(tree->rates->model_log_rates == YES) ratio -= LOG(new_mu/cur_mu);
      
      ratio = EXP(ratio);
      alpha = MIN(1.,ratio);
      
      u = Uni();
      
      if(u > alpha) /* Reject */
	{
	  if(tree->rates->model_log_rates == YES)
	    tree->rates->br_r[d->num] = LOG(cur_mu);
	  else
	    tree->rates->br_r[d->num] = cur_mu;
	  
	  tree->c_lnL        = cur_lnL_data;
	  tree->rates->c_lnL = cur_lnL_rate;
	  
	  Restore_Br_Len(tree);
	  
	  if(tree->mcmc->use_data && tree->io->lk_approx == EXACT)
	    {
	      Update_PMat_At_Given_Edge(b1,tree);
	      if(!d->tax)
		{
		  Update_PMat_At_Given_Edge(b2,tree);
		  Update_PMat_At_Given_Edge(b3,tree);
		}
	      Update_P_Lk(tree,b1,d);
	    }
	  
	  /* tree->both_sides = YES; */
	  /* new_lnL_data = Lk(tree); */
	  /* tree->both_sides = NO; */
	  
	}
      else
	{
	  tree->mcmc->acc_move[tree->mcmc->num_move_br_r+d->num]++;
	}
    }
  tree->mcmc->run_move[tree->mcmc->num_move_br_r+d->num]++;
  
  if(traversal == YES)
    {
      if(d->tax == YES) return;
      else
	{
	  For(i,3)
	    if(d->v[i] != a && d->b[i] != tree->e_root)
	      {
		if(tree->io->lk_approx == EXACT && tree->mcmc->use_data) Update_P_Lk(tree,d->b[i],d);
		/* if(tree->io->lk_approx == EXACT && tree->mcmc->use_data) {tree->both_sides = YES; Lk(tree); } */
		MCMC_One_Rate(d,d->v[i],YES,tree);
	      }
	}
      if(tree->io->lk_approx == EXACT && tree->mcmc->use_data) Update_P_Lk(tree,b,d);
      /* if(tree->io->lk_approx == EXACT && tree->mcmc->use_data) {tree->both_sides = YES; Lk(tree); } */
    }
}

/*********************************************************/

void MCMC_One_Node_Rate(t_node *a, t_node *d, int traversal, t_tree *tree)
{
  t_edge *b;
  int i;

  b = NULL;
  if(a == tree->n_root) b = tree->e_root;
  else
    For(i,3) if(d->v[i] == a) { b = d->b[i]; break; }

  /* Only the LOG_RANDWALK move seems to work here. Change with caution then. */
  tree->rates->br_do_updt[d->num] = YES;
  MCMC_Single_Param_Generic(&(tree->rates->nd_r[d->num]),
			    tree->rates->min_rate,
			    tree->rates->max_rate,
			    tree->mcmc->num_move_nd_r+d->num,
			    &(tree->rates->c_lnL),NULL,
			    Wrap_Lk_Rates,NULL,MCMC_MOVE_LOG_RANDWALK,YES,NULL,tree,NULL);


  Update_PMat_At_Given_Edge(b,tree);

  if(traversal == YES)
    {
      if(d->tax == YES) return;
      else
	{
	  For(i,3)
	    if(d->v[i] != a && d->b[i] != tree->e_root)
	      {
		MCMC_One_Node_Rate(d,d->v[i],YES,tree);
	      }
	}
    }
}

/*********************************************************/

void MCMC_One_Time(t_node *a, t_node *d, int traversal, t_tree *tree)
{
  phydbl u;
  phydbl t_min,t_max;
  phydbl t1_cur, t1_new;
  phydbl cur_lnL_data, new_lnL_data;
  phydbl cur_lnL_rate, new_lnL_rate;
  phydbl ratio,alpha;
  t_edge *b1,*b2,*b3;
  int    i;
  phydbl t0,t2,t3;
  t_node *v2,*v3;
  phydbl K;
  int move_num;


  if(d->tax) return; /* Won't change time at tip */
  if(d == tree->n_root) return; /* Won't change time at root. Use tree height move instead */

  if(FABS(tree->rates->t_prior_min[d->num] - tree->rates->t_prior_max[d->num]) < 1.E-10) return;

  Record_Br_Len(tree);
  RATES_Record_Rates(tree);
  RATES_Record_Times(tree);

  move_num       = d->num-tree->n_otu+tree->mcmc->num_move_nd_t;
  K              = tree->mcmc->tune_move[move_num];
  cur_lnL_data   = tree->c_lnL;
  cur_lnL_rate   = tree->rates->c_lnL;
  t1_cur         = tree->rates->nd_t[d->num];
  new_lnL_data   = cur_lnL_data;
  new_lnL_rate   = cur_lnL_rate;
  ratio          = 0.0;


  v2 = v3 = NULL;
  For(i,3)
    if((d->v[i] != a) && (d->b[i] != tree->e_root))
      {
	if(!v2) { v2 = d->v[i]; }
	else    { v3 = d->v[i]; }
      }


  b1 = NULL;
  if(a == tree->n_root) b1 = tree->e_root;
  else For(i,3) if(d->v[i] == a) { b1 = d->b[i]; break; }

  b2 = b3 = NULL;
  For(i,3)
    if((d->v[i] != a) && (d->b[i] != tree->e_root))
      {
	if(!b2) { b2 = d->b[i]; }
	else    { b3 = d->b[i]; }
      }

  
  t0 = (a)?(tree->rates->nd_t[a->num]):(tree->rates->t_prior_min[d->num]);
  t2 = tree->rates->nd_t[v2->num];
  t3 = tree->rates->nd_t[v3->num];

  t_min = MAX(t0,tree->rates->t_prior_min[d->num]);
  t_max = MIN(MIN(t2,t3),tree->rates->t_prior_max[d->num]);

  t_min += tree->rates->min_dt;
  t_max -= tree->rates->min_dt;

  if(t_min > t_max) 
    {
      PhyML_Printf("\n. t_min = %f t_max = %f",t_min,t_max);
      PhyML_Printf("\n. prior_min = %f prior_max = %f",tree->rates->t_prior_min[d->num],tree->rates->t_prior_max[d->num]);
      PhyML_Printf("\n. Err in file %s at line %d\n",__FILE__,__LINE__);
      /* Exit("\n"); */
    }

  u = Uni();
  /* mult = EXP(K*(u-0.5)); */
  /* t1_new = t1_cur * mult; */

  t1_new = u*(t_max - t_min) + t_min;

  /* t1_new = Rnorm(t1_cur,K); */
  /* t1_new = Reflect(t1_new,t_min,t_max); */

  if(t1_new > t_min && t1_new < t_max) 
    {
      tree->rates->nd_t[d->num] = t1_new;

      /* Update branch lengths */
      tree->rates->br_do_updt[d->num]  = YES;
      tree->rates->br_do_updt[v2->num] = YES;
      tree->rates->br_do_updt[v3->num] = YES;
      RATES_Update_Cur_Bl(tree);

      if(tree->mcmc->use_data)
	{
	  if(tree->io->lk_approx == EXACT)
	    {
	      Update_PMat_At_Given_Edge(b1,tree);
	      Update_PMat_At_Given_Edge(b2,tree);
	      Update_PMat_At_Given_Edge(b3,tree);
	      Update_P_Lk(tree,b1,d);
	    }
	  new_lnL_data = Lk_At_Given_Edge(b1,tree);
	  /* new_lnL_data = Lk(tree); */
	}

      new_lnL_rate = RATES_Lk_Rates(tree);
      
      /* ratio += LOG(mult); */
      if(tree->mcmc->use_data) ratio += (new_lnL_data - cur_lnL_data);
      ratio += (new_lnL_rate - cur_lnL_rate);
      

      ratio = EXP(ratio);
      alpha = MIN(1.,ratio);
      u = Uni();
      
      if(u > alpha) /* Reject */
	{
	  tree->c_lnL        = cur_lnL_data;
	  tree->rates->c_lnL = cur_lnL_rate;
	  Restore_Br_Len(tree);
	  RATES_Reset_Rates(tree);
	  RATES_Reset_Times(tree);

	  if(tree->io->lk_approx == EXACT && tree->mcmc->use_data) 
	    {
	      Update_PMat_At_Given_Edge(b1,tree);
	      Update_PMat_At_Given_Edge(b2,tree);
	      Update_PMat_At_Given_Edge(b3,tree);
	      Update_P_Lk(tree,b1,d);
	    }
	}
      else
	{
	  tree->mcmc->acc_move[move_num]++;
	}
      
      if(t1_new < t0)
	{
	  t1_new = t0+1.E-4;
	  PhyML_Printf("\n");
	  PhyML_Printf("\n. a is root -> %s",(a == tree->n_root)?("YES"):("NO"));
	  PhyML_Printf("\n. t0 = %f t1_new = %f",t0,t1_new);
	  PhyML_Printf("\n. t_min=%f t_max=%f",t_min,t_max);
	  PhyML_Printf("\n. (t1-t0)=%f (t2-t1)=%f",t1_cur-t0,t2-t1_cur);
	  PhyML_Printf("\n. Err in file %s at line %d\n",__FILE__,__LINE__);
	  /*       Exit("\n"); */
	}
      if(t1_new > MIN(t2,t3))
	{
	  PhyML_Printf("\n");
	  PhyML_Printf("\n. a is root -> %s",(a == tree->n_root)?("YES"):("NO"));
	  PhyML_Printf("\n. t0 = %f t1_new = %f t1 = %f t2 = %f t3 = %f MIN(t2,t3)=%f",t0,t1_new,t1_cur,t2,t3,MIN(t2,t3));
	  PhyML_Printf("\n. t_min=%f t_max=%f",t_min,t_max);
	  PhyML_Printf("\n. (t1-t0)=%f (t2-t1)=%f",t1_cur-t0,t2-t1_cur);
	  PhyML_Printf("\n. Err in file %s at line %d\n",__FILE__,__LINE__);
	  /*       Exit("\n"); */
	}
      
      if(isnan(t1_new))
	{
	  PhyML_Printf("\n. run=%d",tree->mcmc->run);
	  PhyML_Printf("\n. Err in file %s at line %d\n",__FILE__,__LINE__);
	  /*       Exit("\n"); */
	}
    }
  
  tree->mcmc->run_move[move_num]++;

  if(traversal == YES)
    {
      if(d->tax == YES) return;
      else
	For(i,3)
	  if(d->v[i] != a && d->b[i] != tree->e_root)
	    {
	      if(tree->io->lk_approx == EXACT && tree->mcmc->use_data) Update_P_Lk(tree,d->b[i],d);
	      /* if(tree->io->lk_approx == EXACT && tree->mcmc->use_data) {tree->both_sides = YES; Lk(tree); } */
	      MCMC_One_Time(d,d->v[i],YES,tree);
	    }
      if(tree->io->lk_approx == EXACT && tree->mcmc->use_data) Update_P_Lk(tree,b1,d);
      /* if(tree->io->lk_approx == EXACT && tree->mcmc->use_data) {tree->both_sides = YES; Lk(tree); } */
    }	    
}

/*********************************************************/

void MCMC_Tree_Height(t_tree *tree)
{
  int i;
  phydbl K,mult,u,alpha,ratio;
  phydbl cur_lnL_data,new_lnL_data;
  phydbl cur_lnL_rate,new_lnL_rate;
  phydbl floor;
  int n_nodes;
  phydbl cur_height, new_height;


  RATES_Record_Times(tree);
  Record_Br_Len(tree);

  K = tree->mcmc->tune_move[tree->mcmc->num_move_tree_height];
  cur_lnL_data = tree->c_lnL;
  new_lnL_data = tree->c_lnL;

  cur_lnL_rate = tree->rates->c_lnL;
  new_lnL_rate = tree->rates->c_lnL;

  cur_height   = tree->rates->nd_t[tree->n_root->num];
  
  u = Uni();
  mult = EXP(K*(u-0.5));
    
  floor = tree->rates->t_floor[tree->n_root->num];

  Scale_Subtree_Height(tree->n_root,mult,floor,&n_nodes,tree);
  new_height = tree->rates->nd_t[tree->n_root->num];

  For(i,2*tree->n_otu-1)
    {
      if(tree->rates->nd_t[i] > tree->rates->t_prior_max[i] ||
  	 tree->rates->nd_t[i] < tree->rates->t_prior_min[i])
  	{
  	  RATES_Reset_Times(tree);
	  Restore_Br_Len(tree);
	  tree->mcmc->run_move[tree->mcmc->num_move_tree_height]++;
  	  return;
  	}
    }
  
  For(i,2*tree->n_otu-2) tree->rates->br_do_updt[i] = YES;
  RATES_Update_Cur_Bl(tree);
  if(tree->mcmc->use_data) new_lnL_data = Lk(tree);

  new_lnL_rate = RATES_Lk_Rates(tree);


  /* The Hastings ratio is actually mult^(n) when changing the absolute
     node heights. When considering the relative heights, this ratio combined
     to the Jacobian for the change of variable ends up to being equal to mult. 
  */
  ratio = 0.0;
  ratio += LOG(mult);
  if(tree->mcmc->use_data) ratio += (new_lnL_data - cur_lnL_data);
  ratio += (new_lnL_rate - cur_lnL_rate);

  /* !!!!!!!!!!!!1 */
  /* ratio += LOG(Dexp(FABS(new_height-floor),1./10.) / Dexp(FABS(cur_height-floor),1./10.)); */

  
  ratio = EXP(ratio);
  alpha = MIN(1.,ratio);
  u = Uni();
  
  
  if(u > alpha)
    {
      RATES_Reset_Times(tree);
      Restore_Br_Len(tree);
      tree->c_lnL = cur_lnL_data;
      tree->rates->c_lnL = cur_lnL_rate;
    }
  else
    {
      tree->mcmc->acc_move[tree->mcmc->num_move_tree_height]++;
      tree->mcmc->acc_move[tree->mcmc->num_move_nd_t+tree->n_root->num-tree->n_otu]++;
    }

  tree->mcmc->run_move[tree->mcmc->num_move_tree_height]++;
  tree->mcmc->run_move[tree->mcmc->num_move_nd_t+tree->n_root->num-tree->n_otu]++;
}

/*********************************************************/

void MCMC_Subtree_Height(t_tree *tree)
{
  int i;
  phydbl K,mult,u,alpha,ratio;
  phydbl cur_lnL_data,new_lnL_data;
  phydbl cur_lnL_rate,new_lnL_rate;
  phydbl cur_height,new_height;
  phydbl floor;
  int target;
  int n_nodes;

  RATES_Record_Times(tree);
  Record_Br_Len(tree);

  K = tree->mcmc->tune_move[tree->mcmc->num_move_subtree_height];
  cur_lnL_data = tree->c_lnL;
  new_lnL_data = tree->c_lnL;
  cur_lnL_rate = tree->rates->c_lnL;
  new_lnL_rate = tree->rates->c_lnL;

  u = Uni();
  mult = EXP(K*(u-0.5));

  target = Rand_Int(tree->n_otu,2*tree->n_otu-3);

  cur_height = tree->rates->nd_t[target];
  floor = tree->rates->t_floor[target];

  if(tree->noeud[target] == tree->n_root)
    {
      PhyML_Printf("\n. Err in file %s at line %d\n",__FILE__,__LINE__);
      Warn_And_Exit("");
    }

  if(!Scale_Subtree_Height(tree->noeud[target],mult,floor,&n_nodes,tree))
    {
      RATES_Reset_Times(tree);
      Restore_Br_Len(tree);
      tree->mcmc->run_move[tree->mcmc->num_move_subtree_height]++;
      return;
    }

  new_height = tree->rates->nd_t[target];

  For(i,2*tree->n_otu-1)
    {
      if(tree->rates->nd_t[i] > tree->rates->t_prior_max[i] ||
  	 tree->rates->nd_t[i] < tree->rates->t_prior_min[i])
  	{
  	  RATES_Reset_Times(tree);
	  Restore_Br_Len(tree);
	  tree->mcmc->run_move[tree->mcmc->num_move_subtree_height]++;
  	  return;
  	}
    }
  
  For(i,2*tree->n_otu-2) tree->rates->br_do_updt[i] = YES;
  RATES_Update_Cur_Bl(tree);
  if(tree->mcmc->use_data) new_lnL_data = Lk(tree);

  new_lnL_rate = RATES_Lk_Rates(tree);

  /* The Hastings ratio here is mult^(n_nodes) and the ratio of the prior joint densities
     of the modified node heigths given the unchanged one is 1. This is different from the 
     case where all the nodes, including the root node, are scaled. 
  */
  ratio = 0.0;
  ratio += (n_nodes) * LOG(mult);
  if(tree->mcmc->use_data) ratio += (new_lnL_data - cur_lnL_data);
  ratio += (new_lnL_rate - cur_lnL_rate);

  ratio = EXP(ratio);
  alpha = MIN(1.,ratio);
  u = Uni();
  
  if(u > alpha)
    {
      RATES_Reset_Times(tree);
      Restore_Br_Len(tree);
      tree->c_lnL = cur_lnL_data;
      tree->rates->c_lnL = cur_lnL_rate;
    }
  else
    {
      tree->mcmc->acc_move[tree->mcmc->num_move_subtree_height]++;
    }

  tree->mcmc->run_move[tree->mcmc->num_move_subtree_height]++;

}

/*********************************************************/

void MCMC_Tree_Rates(t_tree *tree)
{
  phydbl K,mult,u,alpha,ratio;
  phydbl cur_lnL_rate,new_lnL_rate;
  phydbl cur_lnL_data,new_lnL_data;
  int n_nodes;
  phydbl init_clock;
  
  RATES_Record_Rates(tree);
  Record_Br_Len(tree);

  K             = tree->mcmc->tune_move[tree->mcmc->num_move_tree_rates];
  cur_lnL_data  = tree->c_lnL;
  new_lnL_data  = tree->c_lnL;
  cur_lnL_rate  = tree->rates->c_lnL;
  new_lnL_rate  = tree->rates->c_lnL;
  init_clock    = tree->rates->clock_r;

  u = Uni();
  mult = EXP(K*(u-0.5));
    
  /* Multiply branch rates */
  if(!Scale_Subtree_Rates(tree->n_root,mult,&n_nodes,tree))
    {
      RATES_Reset_Rates(tree);
      Restore_Br_Len(tree);
      tree->mcmc->run_move[tree->mcmc->num_move_tree_rates]++;
      return;
    }

  if(n_nodes != 2*tree->n_otu-2)
    {
      PhyML_Printf("\n. Err in file %s at line %d\n",__FILE__,__LINE__);
      Warn_And_Exit("");
    }

  tree->rates->clock_r /= mult;
  if(tree->rates->clock_r < tree->rates->min_clock || tree->rates->clock_r > tree->rates->max_clock)
    {
      tree->rates->clock_r = init_clock;
      RATES_Reset_Rates(tree);
      Restore_Br_Len(tree);
      tree->mcmc->run_move[tree->mcmc->num_move_tree_rates]++;
      return;
    }

  if(tree->rates->model == GUINDON)
    {
      int i;
      For(i,2*tree->n_otu-2) tree->rates->br_do_updt[i] = YES;
      RATES_Update_Cur_Bl(tree);
      if(tree->mcmc->use_data) new_lnL_data = Lk(tree);
    }

  new_lnL_rate = RATES_Lk_Rates(tree);

  ratio = 0.0;
  /* Proposal ratio: 2n-2=> number of multiplications, 1=>number of divisions */
  ratio += (+(2*tree->n_otu-2)-1)*LOG(mult);
  /* ratio += (2*tree->n_otu-2-1-2)*LOG(mult); */
  /* ratio += LOG(mult); */
  /* ratio += (2*tree->n_otu-2)*LOG(mult); */
  /* Prior density ratio */
  ratio += (new_lnL_rate - cur_lnL_rate);
  /* Likelihood density ratio */
  ratio += (new_lnL_data - cur_lnL_data);

  /* If modelling log of rates instead of rates */
  if(tree->rates->model_log_rates == YES) ratio -= (2*tree->n_otu-2)*LOG(mult);


  ratio = EXP(ratio);
  alpha = MIN(1.,ratio);
  u = Uni();
  
  /* printf("\n. cur_lnL=%f new_lnL=%f ratio=%f mult=%f %f [%f %f]", */
  /* 	 cur_lnL_rate,new_lnL_rate,ratio,mult,(2*tree->n_otu-2-1)*LOG(mult),new_lnL_data,cur_lnL_data); */

  if(u > alpha)
    {
      /* PhyML_Printf("\n. Reject mult=%f",mult); */
      tree->rates->clock_r = init_clock;
      RATES_Reset_Rates(tree);
      Restore_Br_Len(tree);
      tree->rates->c_lnL = cur_lnL_rate;
      tree->c_lnL        = cur_lnL_data;
    }
  else
    {
      /* PhyML_Printf("\n. Accept mult=%f",mult); */
      tree->mcmc->acc_move[tree->mcmc->num_move_tree_rates]++;
    }

  tree->mcmc->run_move[tree->mcmc->num_move_tree_rates]++;
}

/*********************************************************/

void MCMC_Subtree_Rates(t_tree *tree)
{  phydbl K,mult,u,alpha,ratio;
  phydbl cur_lnL_rate,new_lnL_rate;
  phydbl cur_lnL_data,new_lnL_data;
  int target;
  int n_nodes;
  
  RATES_Record_Rates(tree);
  Record_Br_Len(tree);

  K             = tree->mcmc->tune_move[tree->mcmc->num_move_subtree_rates];
  cur_lnL_rate  = tree->rates->c_lnL;
  new_lnL_rate  = cur_lnL_rate;
  cur_lnL_data  = tree->c_lnL;
  new_lnL_data  = cur_lnL_data;

  u = Uni();
  mult = EXP(K*(u-0.5));

  target = Rand_Int(tree->n_otu,2*tree->n_otu-3);

  /* Multiply branch rates */
  if(!Scale_Subtree_Rates(tree->noeud[target],mult,&n_nodes,tree))
    {
      RATES_Reset_Rates(tree);
      Restore_Br_Len(tree);
      tree->mcmc->run_move[tree->mcmc->num_move_subtree_rates]++;
      return;
    }

  new_lnL_rate = RATES_Lk_Rates(tree);
  if(tree->mcmc->use_data) new_lnL_data = Lk(tree);

  ratio = 0.0;
  /* Proposal ratio: 2n-2=> number of multiplications, 1=>number of divisions */
  ratio += (+n_nodes)*LOG(mult);
  /* Prior density ratio */
  ratio += (new_lnL_rate - cur_lnL_rate);
  /* Likelihood density ratio */
  ratio += (new_lnL_data - cur_lnL_data);

  /* If modelling log of rates instead of rates */
  if(tree->rates->model_log_rates == YES) ratio -= (n_nodes)*LOG(mult);

  if(tree->rates->model == GUINDON) ratio -= (n_nodes)*LOG(mult);

  ratio = EXP(ratio);
  alpha = MIN(1.,ratio);
  u = Uni();
  
  if(u > alpha)
    {
      RATES_Reset_Rates(tree);
      Restore_Br_Len(tree);
      tree->rates->c_lnL = cur_lnL_rate;
      tree->c_lnL        = cur_lnL_data;
    }
  else
    {
      tree->mcmc->acc_move[tree->mcmc->num_move_subtree_rates]++;
    }

  tree->mcmc->run_move[tree->mcmc->num_move_subtree_rates]++;
}

/*********************************************************/

void MCMC_Swing(t_tree *tree)
{
  int i;
  phydbl K,mult,u,alpha,ratio;
  phydbl cur_lnL_data,new_lnL_data;
  phydbl cur_lnL_rate,new_lnL_rate;

  RATES_Record_Times(tree);
  RATES_Record_Rates(tree);
  Record_Br_Len(tree);

  K             = 3.;
  cur_lnL_data  = tree->c_lnL;
  new_lnL_data  = cur_lnL_data;
  cur_lnL_rate  = tree->rates->c_lnL;
  new_lnL_rate  = cur_lnL_rate;
  ratio         = 0.0;

  u = Uni();
  /* mult = EXP(K*(u-0.5)); */
  mult = u*(K - 1./K) + 1./K;


  For(i,2*tree->n_otu-1)
    {
      if(tree->noeud[i]->tax == NO) 
	{
	  tree->rates->nd_t[i] *= mult;
	}

      if(tree->rates->nd_t[i] > tree->rates->t_prior_max[i] ||
         tree->rates->nd_t[i] < tree->rates->t_prior_min[i])
        {
          RATES_Reset_Times(tree);
          Restore_Br_Len(tree);
          return;
        }
    }

  For(i,2*tree->n_otu-2)
    {
      tree->rates->br_r[i] /= mult;

      if(tree->rates->br_r[i] > tree->rates->max_rate ||
         tree->rates->br_r[i] < tree->rates->min_rate)
        {
          RATES_Reset_Times(tree);
          RATES_Reset_Rates(tree);
          Restore_Br_Len(tree);
          return;
        }
    }

  For(i,2*tree->n_otu-2) tree->rates->br_do_updt[i] = YES;
  RATES_Update_Cur_Bl(tree);
  if(tree->mcmc->use_data) new_lnL_data = Lk(tree);

  new_lnL_rate = RATES_Lk_Rates(tree);

  ratio += (-(tree->n_otu-1.)-2.)*LOG(mult);
  ratio += (new_lnL_rate - cur_lnL_rate);
  if(tree->mcmc->use_data) ratio += (new_lnL_data - cur_lnL_data);

  ratio = EXP(ratio);
  alpha = MIN(1.,ratio);
  u = Uni();

  if(u > alpha)
    {
      RATES_Reset_Times(tree);
      RATES_Reset_Rates(tree);
      Restore_Br_Len(tree);
      tree->c_lnL = cur_lnL_data;
      tree->rates->c_lnL = cur_lnL_rate;
      /* printf("\n. Reject %8f",mult); */
    }
  else
    {
      /* printf("\n. Accept %8f",mult); */
    }


}

/*********************************************************/

void MCMC_Updown_Nu_Cr(t_tree *tree)
{
  phydbl K,mult,u,alpha,ratio;
  phydbl cur_lnL_rate,new_lnL_rate;
  phydbl cur_lnL_data,new_lnL_data;
  int i;
  int n_nodes;

  RATES_Record_Rates(tree);
  Record_Br_Len(tree);

  K             = tree->mcmc->tune_move[tree->mcmc->num_move_updown_nu_cr];
  cur_lnL_data  = tree->c_lnL;
  new_lnL_data  = tree->c_lnL;
  cur_lnL_rate  = tree->rates->c_lnL;
  new_lnL_rate  = tree->rates->c_lnL;
  n_nodes       = 0;

  u = Uni();
  mult = EXP(K*(u-0.5));

  /* Multiply branch rates */
  /* if(!Scale_Subtree_Rates(tree->n_root,mult,&n_nodes,tree)) */
  /*   { */
  /*     RATES_Reset_Rates(tree); */
  /*     Restore_Br_Len(tree); */
  /*     tree->mcmc->run_move[tree->mcmc->num_move_updown_nu_cr]++; */
  /*     return; */
  /*   } */

  tree->rates->clock_r /= mult;
  if(tree->rates->clock_r < tree->rates->min_clock || tree->rates->clock_r > tree->rates->max_clock)
    {
      tree->rates->clock_r *= mult;
      RATES_Reset_Rates(tree);
      Restore_Br_Len(tree);
      tree->mcmc->run_move[tree->mcmc->num_move_updown_nu_cr]++;
      return;
    }

  tree->rates->nu *= mult;
  if(tree->rates->nu < tree->rates->min_nu || tree->rates->nu > tree->rates->max_nu)
    {
      tree->rates->nu /= mult;
      RATES_Reset_Rates(tree);
      Restore_Br_Len(tree);
      tree->mcmc->run_move[tree->mcmc->num_move_updown_nu_cr]++;
      return;
    }
  
  For(i,2*tree->n_otu-2) tree->rates->br_do_updt[i] = YES;
  RATES_Update_Cur_Bl(tree);
  if(tree->mcmc->use_data) new_lnL_data = Lk(tree);

  new_lnL_rate = RATES_Lk_Rates(tree);

  ratio = 0.0;
  /* Proposal ratio: 2n-2=> number of multiplications, 1=>number of divisions */
  /* ratio += n_nodes*LOG(mult); /\* (1-1)*LOG(mult); *\/ */
  ratio += 0.0*LOG(mult); /* (1-1)*LOG(mult); */
  /* Prior density ratio */
  ratio += (new_lnL_rate - cur_lnL_rate);
  /* Likelihood density ratio */
  ratio += (new_lnL_data - cur_lnL_data);

  ratio = EXP(ratio);
  alpha = MIN(1.,ratio);
  u = Uni();
  
  if(u > alpha)
    {
      /* printf("\n. Reject: %f",mult); */
      tree->rates->clock_r *= mult;
      tree->rates->nu /= mult;
      RATES_Reset_Rates(tree);
      Restore_Br_Len(tree);
      tree->rates->c_lnL = cur_lnL_rate;
      tree->c_lnL        = cur_lnL_data;
    }
  else
    {
      /* printf("\n. Accept: %f",mult); */
      tree->mcmc->acc_move[tree->mcmc->num_move_updown_nu_cr]++;
    }

  tree->mcmc->run_move[tree->mcmc->num_move_updown_nu_cr]++;
}

/*********************************************************/

void MCMC_Print_Param_Stdin(t_mcmc *mcmc, t_tree *tree)
{
  time_t cur_time;
  phydbl min;
  int i;
  time(&cur_time);
  
  min = MDBL_MAX;
  For(i,tree->n_otu-1)
    {
      /*       printf("\n. %d %f %f %f %f",i, */
      /* 	     tree->mcmc->new_param_val[tree->mcmc->num_move_nd_t+i], */
      /* 	     tree->mcmc->old_param_val[tree->mcmc->num_move_nd_t+i], */
      /* 	     tree->mcmc->ess[tree->mcmc->num_move_nd_t+i], */
      /* 	     tree->mcmc->sum_val[tree->mcmc->num_move_nd_t+i]); */

      if(tree->mcmc->ess[tree->mcmc->num_move_nd_t+i] < min)
	min = tree->mcmc->ess[tree->mcmc->num_move_nd_t+i];
    }


  if(mcmc->run == 1)
    {
      PhyML_Printf("\n");
      PhyML_Printf("\t%9s","Run");
      PhyML_Printf("\t%6s","Time");
      PhyML_Printf("\t%12s","Likelihood");
      PhyML_Printf("\t%12s","Prior");
      PhyML_Printf("\t%6s","Adjust");    
      PhyML_Printf("\t%18s","SubstRate[ESS]");
      PhyML_Printf("\t%16s","TreeHeight[ESS]");    
      PhyML_Printf("\t%16s","AutoCorrelation");    
      PhyML_Printf("\t%6s","MinESS");    
    }

  if(cur_time - mcmc->t_last_print >  mcmc->print_every)
    {
      mcmc->t_last_print = cur_time;
      PhyML_Printf("\n");
      PhyML_Printf("\t%9d",tree->mcmc->run);
      PhyML_Printf("\t%6d",(int)(cur_time-mcmc->t_beg));
      PhyML_Printf("\t%12.2f",tree->c_lnL);
      PhyML_Printf("\t%12.2f",
		   tree->rates->c_lnL);
		   /* LOG(Dexp(tree->rates->nu,10.))); */
		   /* LOG(Dexp(FABS(tree->rates->nd_t[tree->n_root->num]-tree->rates->t_floor[tree->n_root->num]),10.))); */
      PhyML_Printf("\t%6s",(tree->mcmc->adjust_tuning[0] == 1)?("yes"):("no"));
      PhyML_Printf("\t%12.6f[%4.0f]",RATES_Average_Substitution_Rate(tree),tree->mcmc->ess[tree->mcmc->num_move_clock_r]);
      PhyML_Printf("\t%10.1f[%4.0f]",tree->rates->nd_t[tree->n_root->num],tree->mcmc->ess[tree->mcmc->num_move_nd_t+tree->n_root->num-tree->n_otu]);
      PhyML_Printf("\t%12.4f",tree->rates->nu);
      PhyML_Printf("\t%6.0f",min);
    }
}

/*********************************************************/

void MCMC_Print_Param(t_mcmc *mcmc, t_tree *tree)
{
  int i;
  FILE *fp;
  char *s;
  int orig_approx;
  phydbl orig_lnL;

  if(tree->mcmc->run > mcmc->chain_len) return;

  s = (char *)mCalloc(100,sizeof(char));

  fp = mcmc->out_fp_stats;
  
/*   if(tree->mcmc->run == 0) */
/*     { */
/*       PhyML_Fprintf(stdout," ["); */
/*       fflush(NULL); */
/*     } */
  
/*   if(!(mcmc->run%(mcmc->chain_len/10))) */
/*     { */
/*       PhyML_Fprintf(stdout,"."); */
/*       fflush(NULL); */
/*     } */

/*   if(tree->mcmc->run == mcmc->chain_len) */
/*     { */
/*       PhyML_Fprintf(stdout,"]"); */
/*       fflush(NULL); */
/*     } */


/*   MCMC_Print_Means(mcmc,tree); */
/*   MCMC_Print_Last(mcmc,tree); */




  if(!(mcmc->run%mcmc->sample_interval)) 
    {

      MCMC_Copy_To_New_Param_Val(tree->mcmc,tree);

      For(i,tree->mcmc->n_moves)
	{
	  if((tree->mcmc->acc_rate[i] > .1) && (tree->mcmc->start_ess[i] == NO)) tree->mcmc->start_ess[i] = YES;
	  if(tree->mcmc->start_ess[i] == YES) MCMC_Update_Effective_Sample_Size(i,tree->mcmc,tree);
	  if(tree->mcmc->run > (int) tree->mcmc->chain_len * 0.1) tree->mcmc->adjust_tuning[i] = NO;
	}


      if(tree->mcmc->run == 0)
	{


	  time(&(mcmc->t_beg));
	  time(&(mcmc->t_last_print));


	  PhyML_Fprintf(fp,"# Random seed: %d",tree->io->r_seed);
	  PhyML_Fprintf(fp,"\n");
	  PhyML_Fprintf(fp,"Run\t");
/* 	  PhyML_Fprintf(fp,"Time\t"); */
	  /* PhyML_Fprintf(fp,"MeanRate\t"); */
/* 	  PhyML_Fprintf(fp,"NormFact\t"); */

	  For(i,mcmc->n_moves)
	    {
	      strcpy(s,"Acc.");
	      PhyML_Fprintf(fp,"%s\t",strcat(s,mcmc->move_name[i]));
	    }

	  For(i,mcmc->n_moves)
	    {
	      strcpy(s,"Tune.");
	      PhyML_Fprintf(fp,"%s\t",strcat(s,mcmc->move_name[i]));
	    }

	  /* For(i,mcmc->n_moves) */
	  /*   { */
	  /*     strcpy(s,"Run."); */
	  /*     PhyML_Fprintf(fp,"%s\t",strcat(s,mcmc->move_name[i])); */
	  /*   } */

	  PhyML_Fprintf(fp,"LnLike[Exact]\t");
	  PhyML_Fprintf(fp,"LnLike[Approx]\t");
	  PhyML_Fprintf(fp,"LnPriorRate\t");
	  PhyML_Fprintf(fp,"LnPosterior\t");
	  PhyML_Fprintf(fp,"ClockRate\t");
	  PhyML_Fprintf(fp,"EvolRate\t");
	  PhyML_Fprintf(fp,"Nu\t");
	  PhyML_Fprintf(fp,"TsTv\t");

	  if(fp != stdout)
	    {
	      for(i=tree->n_otu;i<2*tree->n_otu-1;i++)
	  	{
	  	  if(tree->noeud[i] == tree->n_root->v[0])
		    PhyML_Fprintf(fp,"T%d%s\t",i,"[LeftRoot]");
		  else if(tree->noeud[i] == tree->n_root->v[1])
		    PhyML_Fprintf(fp,"T%d%s\t",i,"[RightRoot]");
		  else if(tree->noeud[i] == tree->n_root)
		    PhyML_Fprintf(fp,"T%d%s\t",i,"[Root]");
		  else
		    PhyML_Fprintf(fp,"T%d\t",i);
	  	}
	    }


/* 	  if(fp != stdout) */
/* 	    { */
/* 	      For(i,2*tree->n_otu-1) */
/* 	  	{ */
/* 	  	  if(tree->noeud[i] == tree->n_root->v[0]) */
/* 	  	    PhyML_Fprintf(fp,"R%d[LeftRoot]\t",i); */
/* 	  	  else if(tree->noeud[i] == tree->n_root->v[1]) */
/* 	  	    PhyML_Fprintf(fp,"R%d[RightRoot]\t",i); */
/* 	  	  else if(tree->noeud[i] != tree->n_root) */
/* 	  	    PhyML_Fprintf(fp," R%d[%d]\t",i,tree->noeud[i]->anc->num); */
/* 		  else */
/* 	  	    PhyML_Fprintf(fp," R%d[Root]\t",i); */

/* /\* 		  PhyML_Fprintf(fp," R%d[%f]\t",i,tree->rates->mean_l[i]); *\/ */
/* 	  	} */
/* 	    } */


	  if(fp != stdout)
	    {
	      For(i,2*tree->n_otu-2)
	  	{
	  	  if(tree->noeud[i] == tree->n_root->v[0])
	  	    PhyML_Fprintf(fp,"B%d[LeftRoot]\t",i);
	  	  else if(tree->noeud[i] == tree->n_root->v[1])
	  	    PhyML_Fprintf(fp,"B%d[RightRoot]\t",i);
	  	  else
	  	    PhyML_Fprintf(fp," B%d[%d]\t",i,tree->noeud[i]->anc->num);


/* 		  PhyML_Fprintf(fp," R%d[%f]\t",i,tree->rates->mean_l[i]); */
	  	}
	    }


/* 	  if(fp != stdout) */
/* 	    { */
/* 	      For(i,2*tree->n_otu-3) */
/* 	  	{ */
/* 		  if(tree->t_edges[i] == tree->e_root) */
/* 		    PhyML_Fprintf(fp,"*L[%f]%d\t",i,tree->rates->u_ml_l[i]); */
/* 		  else */
/* 		    PhyML_Fprintf(fp," L[%f]%d\t",i,tree->rates->u_ml_l[i]); */
/* 	  	} */
/* 	    } */


	  PhyML_Fprintf(mcmc->out_fp_trees,"#NEXUS\n");
	  PhyML_Fprintf(mcmc->out_fp_trees,"BEGIN TREES;\n");
	  PhyML_Fprintf(mcmc->out_fp_trees,"\tTRANSLATE\n");
	  For(i,tree->n_otu-1) PhyML_Fprintf(mcmc->out_fp_trees,"\t%3d\t%s,\n",tree->noeud[i]->num,tree->noeud[i]->name);
	  PhyML_Fprintf(mcmc->out_fp_trees,"\t%3d\t%s;\n",tree->noeud[i]->num,tree->noeud[i]->name);
	  tree->write_tax_names = NO;
	}

      PhyML_Fprintf(fp,"\n");

      PhyML_Fprintf(fp,"%6d\t",tree->mcmc->run);

/*       time(&mcmc->t_cur); */
/*       PhyML_Fprintf(fp,"%6d\t",(int)(mcmc->t_cur-mcmc->t_beg)); */
      
/*       RATES_Update_Cur_Bl(tree); */
/*       PhyML_Fprintf(fp,"%f\t",RATES_Check_Mean_Rates(tree)); */

/*       PhyML_Fprintf(fp,"%f\t",tree->rates->norm_fact); */
      For(i,tree->mcmc->n_moves) PhyML_Fprintf(fp,"%f\t",tree->mcmc->acc_rate[i]);
      For(i,tree->mcmc->n_moves) PhyML_Fprintf(fp,"%f\t",(phydbl)(tree->mcmc->tune_move[i]));
/*       For(i,tree->mcmc->n_moves) PhyML_Fprintf(fp,"%d\t",(int)(tree->mcmc->run_move[i])); */

      orig_approx = tree->io->lk_approx;
      orig_lnL = tree->c_lnL;
      tree->io->lk_approx = EXACT;
      if(tree->mcmc->use_data)  Lk(tree);  else tree->c_lnL = 0.0;
      PhyML_Fprintf(fp,"%.1f\t",tree->c_lnL);
      tree->io->lk_approx = NORMAL;
      tree->c_lnL = 0.0;
      if(tree->mcmc->use_data)  Lk(tree);  else tree->c_lnL = 0.0;
      PhyML_Fprintf(fp,"%.1f\t",tree->c_lnL);
      tree->io->lk_approx = orig_approx;
      tree->c_lnL = orig_lnL;

/*       PhyML_Fprintf(fp,"0\t0\t"); */


      PhyML_Fprintf(fp,"%G\t",tree->rates->c_lnL+LOG(Dexp(tree->rates->nu,10.)));
      PhyML_Fprintf(fp,"%G\t",
		    tree->c_lnL+tree->rates->c_lnL);
		    /* LOG(Dexp(tree->rates->nu,10.)) ); */
		    /* LOG(Dexp(FABS(tree->rates->nd_t[tree->n_root->num]-tree->rates->t_floor[tree->n_root->num]),10.))); */
      PhyML_Fprintf(fp,"%G\t",tree->rates->clock_r);
      PhyML_Fprintf(fp,"%G\t",RATES_Average_Substitution_Rate(tree));
      PhyML_Fprintf(fp,"%G\t",tree->rates->nu);
      PhyML_Fprintf(fp,"%G\t",tree->mod->kappa);
      for(i=tree->n_otu;i<2*tree->n_otu-1;i++) PhyML_Fprintf(fp,"%.1f\t",tree->rates->nd_t[i]);
      /* for(i=0;i<2*tree->n_otu-1;i++) PhyML_Fprintf(fp,"%.4f\t",LOG(tree->rates->nd_r[i])); */
      for(i=0;i<2*tree->n_otu-2;i++) PhyML_Fprintf(fp,"%.4f\t",tree->rates->br_r[i]);
      /* if(fp != stdout) for(i=tree->n_otu;i<2*tree->n_otu-1;i++) PhyML_Fprintf(fp,"%G\t",tree->rates->t_prior[i]); */
/*       For(i,2*tree->n_otu-3) PhyML_Fprintf(fp,"%f\t",EXP(tree->t_edges[i]->l)); */
/*       For(i,2*tree->n_otu-3) PhyML_Fprintf(fp,"%f\t",tree->t_edges[i]->l); */
      fflush(NULL);


      // TREES
/*       char *s_tree; */
/* /\*       Branch_Lengths_To_Time_Lengths(tree); *\/ */
/*       Branch_Lengths_To_Rate_Lengths(tree); */
/*       tree->bl_ndigits = 5; */
/* /\*       tree->bl_ndigits = 0; *\/ */
/*       s_tree = Write_Tree(tree); */
/*       tree->bl_ndigits = 7; */
/*       PhyML_Fprintf(mcmc->out_fp_trees,"TREE %8d [%f] = [&R] %s\n",mcmc->run,tree->c_lnL,s_tree); */
/*       Free(s_tree); */
/*       RATES_Update_Cur_Bl(tree); */

    }

  if(tree->mcmc->run == mcmc->chain_len)
    {
      PhyML_Fprintf(mcmc->out_fp_trees,"END;\n");
      fflush(NULL); 
    }
  
  Free(s);
  
}

/*********************************************************/

void MCMC_Print_Means(t_mcmc *mcmc, t_tree *tree)
{

  if(!(mcmc->run%mcmc->sample_interval)) 
    {
      int i;
      char *s;

      s = (char *)mCalloc(T_MAX_FILE,sizeof(char));

      strcpy(s,tree->mcmc->out_filename);
      strcat(s,".means");
      
      fclose(mcmc->out_fp_means);

      mcmc->out_fp_means = fopen(s,"w");
      
      PhyML_Fprintf(mcmc->out_fp_means,"#");
      for(i=tree->n_otu;i<2*tree->n_otu-1;i++) PhyML_Fprintf(mcmc->out_fp_means,"T%d\t",i);	  

      PhyML_Fprintf(mcmc->out_fp_means,"\n");      

      for(i=tree->n_otu;i<2*tree->n_otu-1;i++) tree->rates->t_mean[i] *= (phydbl)(mcmc->run / mcmc->sample_interval);

      for(i=tree->n_otu;i<2*tree->n_otu-1;i++)
	{
	  tree->rates->t_mean[i] += tree->rates->nd_t[i];
	  tree->rates->t_mean[i] /= (phydbl)(mcmc->run / mcmc->sample_interval + 1);

/* 	  PhyML_Fprintf(tree->mcmc->out_fp_means,"%d\t",tree->mcmc->run / tree->mcmc->sample_interval);	   */
	  PhyML_Fprintf(tree->mcmc->out_fp_means,"%.1f\t",tree->rates->t_mean[i]);
	}

      PhyML_Fprintf(tree->mcmc->out_fp_means,"\n");
      fflush(NULL);

      Free(s);
    }
}

/*********************************************************/

void MCMC_Print_Last(t_mcmc *mcmc, t_tree *tree)
{

  if(!(mcmc->run%mcmc->sample_interval)) 
    {
      int i;
      char *s;

      s = (char *)mCalloc(T_MAX_FILE,sizeof(char));

      strcpy(s,tree->mcmc->out_filename);
      strcat(s,".lasts");
      
      fclose(mcmc->out_fp_last);

      mcmc->out_fp_last = fopen(s,"w");

/*       rewind(mcmc->out_fp_last); */

      PhyML_Fprintf(mcmc->out_fp_last,"#");
      PhyML_Fprintf(tree->mcmc->out_fp_last,"Time\t");

      for(i=tree->n_otu;i<2*tree->n_otu-1;i++)
	PhyML_Fprintf(tree->mcmc->out_fp_last,"T%d\t",i);

      PhyML_Fprintf(tree->mcmc->out_fp_last,"\n");

      if(mcmc->run)
	{
	  time(&(mcmc->t_cur));
	  PhyML_Fprintf(tree->mcmc->out_fp_last,"%d\t",(int)(mcmc->t_cur-mcmc->t_beg));
/* 	  PhyML_Fprintf(tree->mcmc->out_fp_last,"%d\t",(int)(mcmc->t_beg)); */
	}

      for(i=tree->n_otu;i<2*tree->n_otu-1;i++) PhyML_Fprintf(tree->mcmc->out_fp_last,"%.1f\t",tree->rates->nd_t[i]);

      PhyML_Fprintf(tree->mcmc->out_fp_last,"\n");
      fflush(NULL);

      Free(s);
  }
}


/*********************************************************/
t_mcmc *MCMC_Make_MCMC_Struct()
{
  t_mcmc *mcmc;

  mcmc               = (t_mcmc *)mCalloc(1,sizeof(t_mcmc));
  mcmc->out_filename = (char *)mCalloc(T_MAX_FILE,sizeof(char));

  return(mcmc);
}

/*********************************************************/

void MCMC_Free_MCMC(t_mcmc *mcmc)
{
  int i;

  Free(mcmc->adjust_tuning);
  Free(mcmc->out_filename);
  Free(mcmc->move_weight);
  Free(mcmc->acc_move);
  Free(mcmc->run_move);
  Free(mcmc->prev_acc_move);
  Free(mcmc->prev_run_move);
  Free(mcmc->acc_rate);
  Free(mcmc->tune_move);
  For(i,mcmc->n_moves) Free(mcmc->move_name[i]);
  Free(mcmc->move_name);
  Free(mcmc->ess_run);
  Free(mcmc->start_ess);
  Free(mcmc->ess);
  Free(mcmc->sum_val);
  Free(mcmc->sum_valsq);
  Free(mcmc->sum_curval_nextval);
  Free(mcmc->first_val);
  Free(mcmc->old_param_val);
  Free(mcmc->new_param_val);
  Free(mcmc);
}

/*********************************************************/

void MCMC_Pause(t_mcmc *mcmc)
{
  char choice;
  char *s;
  int len;

  s = (char *)mCalloc(100,sizeof(char));

  
  if(!(mcmc->run%mcmc->chain_len) && (mcmc->is_burnin == NO))
    {
      PhyML_Printf("\n. Do you wish to stop the analysis [N/y] ");
      if(!scanf("%c",&choice)) Exit("\n");
      if(choice == '\n') choice = 'N';
      else getchar(); /* \n */
      
      Uppercase(&choice);
	  
      switch(choice)
	{
	case 'N': 
	  {	    
	    len = 1E+4;
	    PhyML_Printf("\n. How many extra generations is required [default: 1E+4] ");
	    Getstring_Stdin(s);
	    if(s[0] == '\0') len = 1E+4; 
	    else len = (int)atof(s); 

	    if(len < 0)
	      {
		PhyML_Printf("\n. The value entered must be an integer greater than 0.\n");
		Exit("\n");
	      }	    
	    mcmc->chain_len += len;
	    break;
	  }
	      
	case 'Y': 
	  {
	    PhyML_Printf("\n. Ok. Done.\n");
	    Exit("\n");
	    break;
	  }
	  
	default: 
	  {
	    PhyML_Printf("\n. Please enter 'Y' or 'N'.\n");
	    Exit("\n");
	  }
	}
    }

  Free(s);

}


/*********************************************************/

void MCMC_Terminate()
{
  char choice;
  PhyML_Printf("\n\n. Do you really want to terminate [Y/n]: ");
  if(!scanf("%c",&choice)) Exit("\n");
  if(choice == '\n') choice = 'Y';
  else getchar(); /* \n */      
  Uppercase(&choice);	  
  if(choice == 'Y') raise(SIGTERM);
}


/*********************************************************/

void MCMC_Init_MCMC_Struct(char *filename, t_mcmc *mcmc)
{
  int pid;

  mcmc->is               = NO;
  mcmc->use_data         = YES;
  mcmc->run              = 0;
  mcmc->sample_interval  = 1E+3;
  mcmc->chain_len        = 1E+6;
  mcmc->chain_len_burnin = 1E+5;
  mcmc->randomize        = 1;
  mcmc->norm_freq        = 1E+3;
  mcmc->max_tune         = 1.E+4;
  mcmc->min_tune         = 0.0;
  mcmc->print_every      = 1;
  mcmc->is_burnin        = NO;

  if(filename)
    {
      strcpy(mcmc->out_filename,filename);
      pid = getpid();
      sprintf(mcmc->out_filename+strlen(mcmc->out_filename),".%d",pid);
    }

  if(filename) 
    {
      char *s;

      s = (char *)mCalloc(T_MAX_NAME,sizeof(char));

      mcmc->out_fp_stats = fopen(mcmc->out_filename,"w");

      strcpy(s,mcmc->out_filename);
      strcat(s,".trees");
      mcmc->out_fp_trees = fopen(s,"w");

/*       strcpy(s,tree->mcmc->out_filename); */
/*       strcat(s,".means"); */
/*       tree->mcmc->out_fp_means = fopen(s,"w"); */

/*       strcpy(s,tree->mcmc->out_filename); */
/*       strcat(s,".lasts"); */
/*       tree->mcmc->out_fp_last  = fopen(s,"w"); */

      Free(s);
    }
  else 
    {
      mcmc->out_fp_stats = stderr;
      mcmc->out_fp_trees = stderr;
      /* tree->mcmc->out_fp_means = stderr; */
      /* tree->mcmc->out_fp_last  = stderr; */
    }
}

/*********************************************************/

void MCMC_Copy_MCMC_Struct(t_mcmc *ori, t_mcmc *cpy, char *filename)
{
  int pid;
  int i;
  
  cpy->use_data           = ori->use_data        ;
  cpy->sample_interval    = ori->sample_interval ;
  cpy->chain_len          = ori->chain_len       ;
  cpy->randomize          = ori->randomize       ;
  cpy->norm_freq          = ori->norm_freq       ;
  cpy->n_moves            = ori->n_moves         ;
  cpy->max_tune           = ori->max_tune        ;
  cpy->min_tune           = ori->min_tune        ;
  cpy->print_every        = ori->print_every     ;
  cpy->is_burnin          = ori->is_burnin       ;
  cpy->is                 = ori->is              ;

  For(i,cpy->n_moves) 
    {
      cpy->old_param_val[i]      = ori->old_param_val[i];
      cpy->new_param_val[i]      = ori->new_param_val[i];
      cpy->start_ess[i]          = ori->start_ess[i];
      cpy->ess_run[i]            = ori->ess_run[i];
      cpy->ess[i]                = ori->ess[i];
      cpy->sum_val[i]            = ori->sum_val[i];
      cpy->sum_valsq[i]          = ori->sum_valsq[i];
      cpy->first_val[i]          = ori->first_val[i];
      cpy->sum_curval_nextval[i] = ori->sum_curval_nextval[i];
      cpy->move_weight[i]        = ori->move_weight[i];
      cpy->run_move[i]           = ori->run_move[i];
      cpy->acc_move[i]           = ori->acc_move[i];
      cpy->prev_run_move[i]      = ori->prev_run_move[i];
      cpy->prev_acc_move[i]      = ori->prev_acc_move[i];
      cpy->acc_rate[i]           = ori->acc_rate[i];
      cpy->tune_move[i]          = ori->tune_move[i];
      strcpy(cpy->move_name[i],ori->move_name[i]);
      cpy->adjust_tuning[i]      = ori->adjust_tuning[i];
    }


  
  if(filename) 
    {
      char *s;

      s = (char *)mCalloc(T_MAX_NAME,sizeof(char));

      strcpy(cpy->out_filename,filename);
      pid = getpid();
      sprintf(cpy->out_filename+strlen(cpy->out_filename),".%d",pid);

      cpy->out_fp_stats = fopen(cpy->out_filename,"w");

      strcpy(s,cpy->out_filename);
      strcat(s,".trees");
      cpy->out_fp_trees = fopen(s,"w");

      Free(s);
    }
  else 
    {
      cpy->out_fp_stats = stderr;
      cpy->out_fp_trees = stderr;
    }
}


/*********************************************************/

void MCMC_Close_MCMC(t_mcmc *mcmc)
{
  fclose(mcmc->out_fp_trees);
  fclose(mcmc->out_fp_stats);
  /* fclose(mcmc->out_fp_means); */
  /* fclose(mcmc->out_fp_last); */
}

/*********************************************************/

void MCMC_Randomize_Branch_Lengths(t_tree *tree)
{
  int i;

  if(tree->mod->log_l == NO)
    For(i,2*tree->n_otu-3) tree->t_edges[i]->l = Rexp(10.);
  else
    For(i,2*tree->n_otu-3) tree->t_edges[i]->l = -4* Uni();
}

/*********************************************************/

void MCMC_Randomize_Node_Rates(t_tree *tree)
{

  int i,err;
  phydbl mean_r, var_r;
  phydbl min_r, max_r;

  mean_r = 1.0;
  var_r  = 0.5;
  min_r  = tree->rates->min_rate;
  max_r  = tree->rates->max_rate;

  For(i,2*tree->n_otu-2)
    if(tree->noeud[i] != tree->n_root)
      tree->rates->nd_r[i] = Rnorm_Trunc(mean_r,SQRT(var_r),min_r,max_r,&err);
}

/*********************************************************/

void MCMC_Randomize_Rates(t_tree *tree)
{

  /* Should be called once t_node times have been determined */

  int i;
  phydbl u;
  phydbl r_min,r_max;

  For(i,2*tree->n_otu-2) tree->rates->br_r[i] = 1.0;

  r_min = 0.9;
  r_max = 1.1;
  u     = 0.0;

/*   For(i,2*tree->n_otu-2) */
/*     { */
/*       u = Uni(); */
/*       u = u * (r_max-r_min) + r_min; */
/*       tree->rates->br_r[i] = u; */

/*       if(tree->rates->br_r[i] < tree->rates->min_rate) tree->rates->br_r[i] = tree->rates->min_rate;  */
/*       if(tree->rates->br_r[i] > tree->rates->max_rate) tree->rates->br_r[i] = tree->rates->max_rate;  */
/*     } */

  MCMC_Randomize_Rates_Pre(tree->n_root,tree->n_root->v[0],tree);
  MCMC_Randomize_Rates_Pre(tree->n_root,tree->n_root->v[1],tree);
}

/*********************************************************/

void MCMC_Randomize_Rates_Pre(t_node *a, t_node *d, t_tree *tree)
{
  int i;
  phydbl mean_r, var_r;
  phydbl min_r, max_r;
  int err;

/*   mean_r = tree->rates->br_r[a->num]; */
/*   var_r  = tree->rates->nu * (tree->rates->nd_t[d->num] - tree->rates->nd_t[a->num]); */
  mean_r = 1.0;
  var_r  = 0.5;
  min_r  = tree->rates->min_rate;
  max_r  = tree->rates->max_rate;
  
  tree->rates->br_r[d->num] = Rnorm_Trunc(mean_r,SQRT(var_r),min_r,max_r,&err);

  if(d->tax) return;
  else
    {
      For(i,3)
	if(d->v[i] != a && d->b[i] != tree->e_root)
	  MCMC_Randomize_Rates_Pre(d,d->v[i],tree);
    }
}

/*********************************************************/

void MCMC_Randomize_Nu(t_tree *tree)
{
  phydbl min_nu,max_nu;
  phydbl u;

  min_nu = tree->rates->min_nu;
  max_nu = tree->rates->max_nu;

  u = Uni();
  tree->rates->nu = (max_nu - min_nu) * u + min_nu;
}

/*********************************************************/

void MCMC_Randomize_Clock_Rate(t_tree *tree)
{
  phydbl u;
  u = Uni();
  tree->rates->clock_r = u * (tree->rates->max_clock - tree->rates->min_clock) + tree->rates->min_clock;
}

/*********************************************************/

void MCMC_Randomize_Alpha(t_tree *tree)
{
  phydbl u;

  u = Uni();
  tree->rates->alpha = u*6.0+1.0;
}

/*********************************************************/

void MCMC_Randomize_Node_Times(t_tree *tree)
{
  phydbl t_sup, t_inf;
  phydbl u;
  int iter;
  int i;
  phydbl dt,min_dt;
  int min_node;

  t_inf = tree->rates->t_prior_min[tree->n_root->num];
  t_sup = tree->rates->t_prior_max[tree->n_root->num];

  u = Uni();
  u *= (t_sup - t_inf);
  u += t_inf;
  
  tree->rates->nd_t[tree->n_root->num] = u;


  MCMC_Randomize_Node_Times_Top_Down(tree->n_root,tree->n_root->v[0],tree);
  MCMC_Randomize_Node_Times_Top_Down(tree->n_root,tree->n_root->v[1],tree);
  
  min_node = -1;
  iter = 0;
  do
    {
      min_dt = 1E+5;
      For(i,2*tree->n_otu-2) 
	{
	  dt = tree->rates->nd_t[i] - tree->rates->nd_t[tree->noeud[i]->anc->num];
	  if(dt < min_dt)
	    {
	      min_dt = dt;
	      min_node = i;
	    }
	}

      if(min_dt > -.1 * tree->rates->nd_t[tree->n_root->num]/(phydbl)(tree->n_otu-1)) break;

      MCMC_Randomize_Node_Times_Bottom_Up(tree->n_root,tree->n_root->v[0],tree);
      MCMC_Randomize_Node_Times_Bottom_Up(tree->n_root,tree->n_root->v[1],tree);
      
      iter++;
    }
  while(iter < 200);

  if(iter == 200)
    {      
      PhyML_Printf("\n. min_dt = %f",min_dt);
      PhyML_Printf("\n. min->t=%f min->anc->t=%f",tree->rates->nd_t[min_node],tree->rates->nd_t[tree->noeud[min_node]->anc->num]);
      PhyML_Printf("\n. up=%f down=%f",tree->rates->t_prior_min[min_node],tree->rates->t_floor[tree->noeud[min_node]->anc->num]);
      PhyML_Printf("\n. Err in file %s at line %d\n",__FILE__,__LINE__);
      Warn_And_Exit("");
    }


/*   PhyML_Printf("\n. Needed %d iterations to randomize node heights.",iter); */

/*   TIMES_Print_Node_Times(tree->n_root,tree->n_root->v[0],tree); */
/*   TIMES_Print_Node_Times(tree->n_root,tree->n_root->v[1],tree); */

  if(RATES_Check_Node_Times(tree))
    {
      PhyML_Printf("\n. Err in file %s at line %d\n",__FILE__,__LINE__);
      Warn_And_Exit("");
    }
}

/*********************************************************/

void MCMC_Randomize_Node_Times_Bottom_Up(t_node *a, t_node *d, t_tree *tree)
{
  if(d->tax) return;
  else
    {
      int i;      
      phydbl u;
      phydbl t_inf, t_sup;
      t_node *v1, *v2;


      For(i,3)
	{
	  if((d->v[i] != a) && (d->b[i] != tree->e_root))
	    {
	      MCMC_Randomize_Node_Times_Bottom_Up(d,d->v[i],tree);
	    }
	}

      v1 = v2 = NULL;
      For(i,3)
      	{
      	  if(d->v[i] != a && d->b[i] != tree->e_root)
      	    {
      	      if(!v1) v1 = d->v[i];
      	      else    v2 = d->v[i];
      	    }
      	}

      t_sup = MIN(tree->rates->nd_t[v1->num],tree->rates->nd_t[v2->num]);
      t_inf = tree->rates->nd_t[a->num];

      u = Uni();
      u *= (t_sup - t_inf);
      u += t_inf;
      
      if(u > tree->rates->t_prior_min[d->num] && u < tree->rates->t_prior_max[d->num])
	tree->rates->nd_t[d->num] = u;
    }  
}

/*********************************************************/

void MCMC_Randomize_Node_Times_Top_Down(t_node *a, t_node *d, t_tree *tree)
{
  if(d->tax) return;
  else
    {
      int i;      
      phydbl u;
      phydbl t_inf, t_sup;

      t_inf = MAX(tree->rates->nd_t[a->num],tree->rates->t_prior_min[d->num]);
      t_sup = tree->rates->t_prior_max[d->num];

      u = Uni();
      u *= (t_sup - t_inf);
      u += t_inf;
      
      tree->rates->nd_t[d->num] = u;

      For(i,3)
	{
	  if((d->v[i] != a) && (d->b[i] != tree->e_root))
	    {
	      MCMC_Randomize_Node_Times_Top_Down(d,d->v[i],tree);
	    }
	}
    }
}

/*********************************************************/

void MCMC_Get_Acc_Rates(t_mcmc *mcmc)
{
  int i;
  phydbl eps;
  int lag;


  if(mcmc->run < (int)(0.01*mcmc->chain_len)) lag = 50;
  else lag = 500;

  eps = 1.E-6;

  For(i,mcmc->n_moves)
    {
      if(mcmc->run_move[i] - mcmc->prev_run_move[i] > lag)
	{
	  mcmc->acc_rate[i] = 
	    (phydbl)(mcmc->acc_move[i] - mcmc->prev_acc_move[i] + eps) / 
	    (phydbl)(mcmc->run_move[i] - mcmc->prev_run_move[i] + eps) ;


	  mcmc->prev_run_move[i] = mcmc->run_move[i];
	  mcmc->prev_acc_move[i] = mcmc->acc_move[i];
	  
	  MCMC_Adjust_Tuning_Parameter(i,mcmc);
	}
    }
}

/*********************************************************/

void MCMC_Adjust_Tuning_Parameter(int move, t_mcmc *mcmc)
{
  if(mcmc->adjust_tuning[move])
    {
      phydbl scale;
      phydbl rate;
      phydbl rate_inf,rate_sup;
      
      if(mcmc->run < (int)(0.01*mcmc->chain_len)) scale = 2.0;
      else scale = 1.2;

      if(!strcmp(mcmc->move_name[move],"tree_height"))
	{
	  rate_inf = 0.6;
	  rate_sup = 0.8;
	}
      else
	{
	  rate_inf = 0.2;
	  rate_sup = 0.7;
	}

	      /* PhyML_Printf("\n. %s acc=%d run=%d tune=%f", */
	      /* 		   mcmc->move_name[i], */
	      /* 		   mcmc->acc_move[i], */
	      /* 		   mcmc->run_move[i], */
	      /* 		   mcmc->tune_move[i]); */

      rate = mcmc->acc_rate[move];
      

      if(rate < rate_inf)      mcmc->tune_move[move] /= scale;
      else if(rate > rate_sup) mcmc->tune_move[move] *= scale;
	  
      if(mcmc->tune_move[move] > mcmc->max_tune) mcmc->tune_move[move] = mcmc->max_tune;
      if(mcmc->tune_move[move] < mcmc->min_tune) mcmc->tune_move[move] = mcmc->min_tune;
	  
    }
}

/*********************************************************/

void MCMC_One_Length(t_edge *b, t_tree *tree)
{
  phydbl u;
  phydbl new_lnL_data, cur_lnL_data;
  phydbl ratio, alpha;
  phydbl new_l, cur_l;
  phydbl K,mult;


  cur_l        = b->l;
  cur_lnL_data = tree->c_lnL;
  K            = 0.1;
  
  u = Uni();
  mult = EXP(K*(u-0.5));
  /* mult = u*(K-1./K)+1./K; */
  new_l = cur_l * mult;

  if(new_l < tree->mod->l_min || new_l > tree->mod->l_max) return;

  b->l = new_l;
  new_lnL_data = Lk_At_Given_Edge(b,tree);
/*   tree->both_sides = NO; */
/*   new_lnL_data = Lk(tree); */


  ratio =
    (new_lnL_data - cur_lnL_data) +
    (LOG(mult));


  ratio = EXP(ratio);
  alpha = MIN(1.,ratio);
  
  u = Uni();
  
  if(u > alpha) /* Reject */
    {
      b->l = cur_l;
      Update_PMat_At_Given_Edge(b,tree);
      tree->c_lnL = cur_lnL_data;
    }

}

/*********************************************************/

void MCMC_Scale_Br_Lens(t_tree *tree)
{
  phydbl u;
  phydbl new_lnL_data, cur_lnL_data;
  phydbl ratio, alpha;
  phydbl K,mult;
  int i;

  Record_Br_Len(tree);

  cur_lnL_data = tree->c_lnL;
  K            = 1.2;
  
  u = Uni();
  mult = u*(K-1./K)+1./K;

  For(i,2*tree->n_otu-3) 
    {
      tree->t_edges[i]->l *= mult;
      if(tree->t_edges[i]->l < tree->mod->l_min || 
	 tree->t_edges[i]->l > tree->mod->l_max) return;
    }

  tree->both_sides = NO;
  new_lnL_data = Lk(tree);

  ratio =
    (new_lnL_data - cur_lnL_data) +
    (2*tree->n_otu-5) * (LOG(mult));

  ratio = EXP(ratio);
  alpha = MIN(1.,ratio);
  
  u = Uni();
  
  if(u > alpha) /* Reject */
    {
      Restore_Br_Len(tree);
      tree->c_lnL = cur_lnL_data;
    }
}

/*********************************************************/

void MCMC_Br_Lens(t_tree *tree)
{
  MCMC_Br_Lens_Pre(tree->noeud[0],
  		   tree->noeud[0]->v[0],
  		   tree->noeud[0]->b[0],tree);

  /* int i; */
  /* For(i,2*tree->n_otu-3) */
  /*   { */
  /*     MCMC_One_Length(tree->t_edges[Rand_Int(0,2*tree->n_otu-4)],acc,run,tree); */
  /*   } */
}

/*********************************************************/

void MCMC_Br_Lens_Pre(t_node *a, t_node *d, t_edge *b, t_tree *tree)
{
  int i;


  if(a == tree->n_root || d == tree->n_root)
    {
      PhyML_Printf("\n. Err in file %s at line %d\n",__FILE__,__LINE__);
      Exit("\n");
    }

  MCMC_One_Length(b,tree);
  if(d->tax) return;
  else 
    {
      For(i,3) 
	if(d->v[i] != a)
	  {
	    Update_P_Lk(tree,d->b[i],d);
	    MCMC_Br_Lens_Pre(d,d->v[i],d->b[i],tree);
	  }
      Update_P_Lk(tree,b,d);
    }  
}

/*********************************************************/

void MCMC_Nu(t_tree *tree)
{
  phydbl cur_nu,new_nu,cur_lnL_rate,new_lnL_rate;
  phydbl u,alpha,ratio;
  phydbl min_nu,max_nu;
  phydbl K;
  phydbl cur_lnL_data, new_lnL_data;
  int i;

  Record_Br_Len(tree);

  cur_nu        = -1.0;
  new_nu        = -1.0;
  ratio         = -1.0;

  K = tree->mcmc->tune_move[tree->mcmc->num_move_nu];

  cur_lnL_rate = tree->rates->c_lnL;
  new_lnL_rate = tree->rates->c_lnL;

  cur_lnL_data = tree->c_lnL;
  new_lnL_data = tree->c_lnL;
  
  cur_nu       = tree->rates->nu;

  min_nu = tree->rates->min_nu;
  max_nu = tree->rates->max_nu;

  u = Uni();

  /* mult = EXP(K*(u-0.5)); */
  /* if(K < 1.) K  = 1./K; */
  /* mult = u*(K - 1./K)+1./K; */
  /* new_nu = cur_nu * mult; */
  
  new_nu = Rnorm(cur_nu,K);
  new_nu = Reflect(new_nu,min_nu,max_nu);

  /* new_nu = u*(max_nu - min_nu) + min_nu; */
  /* new_nu = Rexp(1.); */

  /* new_nu = u*(2.*K) + cur_nu - K; */

  if(new_nu < max_nu && new_nu > min_nu) 
    {
      tree->rates->nu = new_nu;
      
      new_lnL_rate = RATES_Lk_Rates(tree);      

      if(tree->rates->model == GUINDON)
	{
	  For(i,2*tree->n_otu-2) tree->rates->br_do_updt[i] = YES;
	  new_lnL_data = Lk(tree);
	}
      
      ratio = 0.0;
      /* ratio += (+1.)*LOG(mult); */
      ratio +=
      	(new_lnL_rate - cur_lnL_rate);
      ratio +=
	(new_lnL_data - cur_lnL_data);


      /* !!!!!!!!!!!!!!!! */
      /* Modelling exp(nu) and making move on nu */
      /* ratio += (new_nu - cur_nu); */
	 
      /* Exponential prior on nu */
      /* ratio += 10.*(cur_nu - new_nu); */

 
      ratio = EXP(ratio);
      alpha = MIN(1.,ratio);
      
      u = Uni();
      if(u > alpha) /* Reject */
	{
	  tree->rates->nu    = cur_nu;
	  tree->rates->c_lnL = cur_lnL_rate;
	  tree->c_lnL        = cur_lnL_data;
	  Restore_Br_Len(tree);
	}
      else
	{
	  tree->mcmc->acc_move[tree->mcmc->num_move_nu]++;
	}
    }
  tree->mcmc->run_move[tree->mcmc->num_move_nu]++;
}

/*********************************************************/

void MCMC_All_Rates(t_tree *tree)
{
  phydbl cur_lnL_data, new_lnL_data, cur_lnL_rate;
  phydbl u, ratio, alpha;

  new_lnL_data = tree->c_lnL;
  cur_lnL_data = tree->c_lnL;
  cur_lnL_rate = tree->rates->c_lnL;
  ratio        = 0.0;
  
  Record_Br_Len(tree);
  RATES_Record_Rates(tree);
  
  MCMC_Sim_Rate(tree->n_root,tree->n_root->v[0],tree);
  MCMC_Sim_Rate(tree->n_root,tree->n_root->v[1],tree);

  new_lnL_data = Lk(tree);
  
  ratio += (new_lnL_data - cur_lnL_data);
  ratio = EXP(ratio);

  alpha = MIN(1.,ratio);
  u = Uni();

  if(u > alpha) /* Reject */
    {
      Restore_Br_Len(tree);
      RATES_Reset_Rates(tree);
      tree->rates->c_lnL = cur_lnL_rate;
    }
  else
    {
      tree->rates->c_lnL = RATES_Lk_Rates(tree);;
    }
}

/*********************************************************/

/* Only works when simulating from prior */
void MCMC_Sim_Rate(t_node *a, t_node *d, t_tree *tree)
{
  int err;
  phydbl mean,sd,br_r_a,dt_d;

  br_r_a = tree->rates->br_r[a->num];
  dt_d = tree->rates->nd_t[d->num] - tree->rates->nd_t[a->num];

  sd   = SQRT(dt_d*tree->rates->nu);
  
  if(tree->rates->model_log_rates == YES)
    {
      mean = br_r_a - .5*sd*sd;
    }
  else
    {
      mean = br_r_a +
	sd*
	(Dnorm(tree->rates->min_rate,br_r_a,sd) - Dnorm(tree->rates->max_rate,br_r_a,sd)) /
	(Pnorm(tree->rates->min_rate,br_r_a,sd) - Pnorm(tree->rates->max_rate,br_r_a,sd)) ;
    }

      
  tree->rates->br_r[d->num] = Rnorm_Trunc(mean,sd,
  					  tree->rates->min_rate,
  					  tree->rates->max_rate,
  					  &err);


  /* tree->rates->br_r[d->num] = Rnorm(LOG(tree->rates->br_r[a->num]), */
  /* 				    SQRT(tree->rates->nu * (tree->rates->nd_t[d->num] - tree->rates->nd_t[a->num]))); */
  /* tree->rates->br_r[d->num] = EXP(tree->rates->br_r[d->num]); */

  
  if(d->tax) return;
  else
    {
      int i;

      For(i,3)
	if(d->v[i] != a && d->b[i] != tree->e_root)
	  MCMC_Sim_Rate(d,d->v[i],tree);
    }
}

/*********************************************************/

void MCMC_Complete_MCMC(t_mcmc *mcmc, t_tree *tree)
{
  int i;
  phydbl sum;

  mcmc->n_moves = 0;


  mcmc->num_move_nd_r = mcmc->n_moves;
  mcmc->n_moves += 2*tree->n_otu-1;

  mcmc->num_move_br_r = mcmc->n_moves;
  mcmc->n_moves += 2*tree->n_otu-2;

  mcmc->num_move_nd_t = mcmc->n_moves;
  mcmc->n_moves += tree->n_otu-1;

  mcmc->num_move_nu             = mcmc->n_moves++;
  mcmc->num_move_clock_r        = mcmc->n_moves++;
  mcmc->num_move_tree_height    = mcmc->n_moves++;
  mcmc->num_move_subtree_height = mcmc->n_moves++;
  mcmc->num_move_kappa          = mcmc->n_moves++;
  mcmc->num_move_tree_rates     = mcmc->n_moves++;
  mcmc->num_move_subtree_rates  = mcmc->n_moves++;
  mcmc->num_move_updown_nu_cr   = mcmc->n_moves++;

  mcmc->run_move           = (int *)mCalloc(mcmc->n_moves,sizeof(int));
  mcmc->acc_move           = (int *)mCalloc(mcmc->n_moves,sizeof(int));
  mcmc->prev_run_move      = (int *)mCalloc(mcmc->n_moves,sizeof(int));
  mcmc->prev_acc_move      = (int *)mCalloc(mcmc->n_moves,sizeof(int));
  mcmc->acc_rate           = (phydbl *)mCalloc(mcmc->n_moves,sizeof(phydbl));
  mcmc->move_weight        = (phydbl *)mCalloc(mcmc->n_moves,sizeof(phydbl));
  
  /* TO DO: instead of n_moves here we should have something like n_param */
  mcmc->sum_val            = (phydbl *)mCalloc(mcmc->n_moves,sizeof(phydbl));
  mcmc->first_val          = (phydbl *)mCalloc(mcmc->n_moves,sizeof(phydbl));
  mcmc->sum_valsq          = (phydbl *)mCalloc(mcmc->n_moves,sizeof(phydbl));
  mcmc->sum_curval_nextval = (phydbl *)mCalloc(mcmc->n_moves,sizeof(phydbl));
  mcmc->ess                = (phydbl *)mCalloc(mcmc->n_moves,sizeof(phydbl));
  mcmc->ess_run            = (int *)mCalloc(mcmc->n_moves,sizeof(int));
  mcmc->start_ess          = (int *)mCalloc(mcmc->n_moves,sizeof(int));
  mcmc->new_param_val      = (phydbl *)mCalloc(mcmc->n_moves,sizeof(phydbl));
  mcmc->old_param_val      = (phydbl *)mCalloc(mcmc->n_moves,sizeof(phydbl));
  mcmc->adjust_tuning      = (int *)mCalloc(mcmc->n_moves,sizeof(int));

  mcmc->move_name = (char **)mCalloc(mcmc->n_moves,sizeof(char *));
  For(i,mcmc->n_moves) mcmc->move_name[i] = (char *)mCalloc(50,sizeof(char));

  For(i,mcmc->n_moves) mcmc->adjust_tuning[i] = YES;

  for(i=mcmc->num_move_br_r;i<mcmc->num_move_br_r+2*tree->n_otu-2;i++) strcpy(mcmc->move_name[i],"br_rate");  
  for(i=mcmc->num_move_nd_r;i<mcmc->num_move_nd_r+2*tree->n_otu-1;i++) strcpy(mcmc->move_name[i],"nd_rate");
  for(i=mcmc->num_move_nd_t;i<mcmc->num_move_nd_t+tree->n_otu-1;i++)   strcpy(mcmc->move_name[i],"time");
  strcpy(mcmc->move_name[mcmc->num_move_nu],"nu");
  strcpy(mcmc->move_name[mcmc->num_move_clock_r],"clock");
  strcpy(mcmc->move_name[mcmc->num_move_tree_height],"tree_height");
  strcpy(mcmc->move_name[mcmc->num_move_subtree_height],"subtree_height");
  strcpy(mcmc->move_name[mcmc->num_move_kappa],"kappa");
  strcpy(mcmc->move_name[mcmc->num_move_tree_rates],"tree_rates");
  strcpy(mcmc->move_name[mcmc->num_move_subtree_rates],"subtree_rates");
  strcpy(mcmc->move_name[mcmc->num_move_updown_nu_cr],"updown_nu_cr");
  
  /* We start with small tuning parameter values in order to have inflated ESS
     for clock_r */
  mcmc->tune_move = (phydbl *)mCalloc(mcmc->n_moves,sizeof(phydbl));
  for(i=mcmc->num_move_br_r;i<mcmc->num_move_br_r+2*tree->n_otu-2;i++) mcmc->tune_move[i] = 2.0; /* Edge rates */
  for(i=mcmc->num_move_nd_r;i<mcmc->num_move_nd_r+2*tree->n_otu-1;i++) mcmc->tune_move[i] = 2.0; /* Node rates */
  for(i=mcmc->num_move_nd_t;i<mcmc->num_move_nd_t+tree->n_otu-1;i++)   mcmc->tune_move[i] = 2.0;  /* Times */
  mcmc->tune_move[mcmc->num_move_clock_r]         = 2.;
  mcmc->tune_move[mcmc->num_move_tree_height]     = 2.;
  mcmc->tune_move[mcmc->num_move_subtree_height]  = 2.;
  mcmc->tune_move[mcmc->num_move_nu]              = 2.;
  mcmc->tune_move[mcmc->num_move_kappa]           = 2.;   
  mcmc->tune_move[mcmc->num_move_tree_rates]      = 2.;   
  mcmc->tune_move[mcmc->num_move_subtree_rates]   = 2.;   
  mcmc->tune_move[mcmc->num_move_updown_nu_cr]    = 2.;   
  
  for(i=mcmc->num_move_br_r;i<mcmc->num_move_br_r+2*tree->n_otu-2;i++) mcmc->move_weight[i] = 1.*(phydbl)(1./(2.*tree->n_otu-2)); /* Rates */
  /* for(i=mcmc->num_move_br_r;i<mcmc->num_move_br_r+2*tree->n_otu-2;i++) mcmc->move_weight[i] = 0.0; /\* Rates *\/ */

  if(tree->rates->model == GUINDON)
    for(i=mcmc->num_move_nd_r;i<mcmc->num_move_nd_r+2*tree->n_otu-1;i++) mcmc->move_weight[i] = (phydbl)(1./(2*tree->n_otu-1)); /* Node rates */
  else
    for(i=mcmc->num_move_nd_r;i<mcmc->num_move_nd_r+2*tree->n_otu-1;i++) mcmc->move_weight[i] = 0.0; /* Node rates */

  for(i=mcmc->num_move_nd_t;i<mcmc->num_move_nd_t+tree->n_otu-1;i++)   mcmc->move_weight[i] = (phydbl)(1./(tree->n_otu-1));  /* Times */
  /* for(i=mcmc->num_move_nd_t;i<mcmc->num_move_nd_t+tree->n_otu-1;i++)   mcmc->move_weight[i] = 0.0;  /\* Times *\/ */
  mcmc->move_weight[mcmc->num_move_clock_r]         = 1.0;
  mcmc->move_weight[mcmc->num_move_tree_height]     = 1.0;
  mcmc->move_weight[mcmc->num_move_subtree_height]  = 1.0;
  mcmc->move_weight[mcmc->num_move_nu]              = 1.0;
  mcmc->move_weight[mcmc->num_move_kappa]           = 0.5;
  mcmc->move_weight[mcmc->num_move_tree_rates]      = 1.0;
  mcmc->move_weight[mcmc->num_move_subtree_rates]   = 1.0;
  mcmc->move_weight[mcmc->num_move_updown_nu_cr]    = 0.0;

  /* For(i,2*tree->n_otu-2) mcmc->move_weight[i] = .0; /\* Rates *\/ */
  /* for(i= 2*tree->n_otu-2; i < tree->n_otu+1+2*tree->n_otu-2; i++) mcmc->move_weight[i] = .0;  /\* Times *\/ */
  /* mcmc->move_weight[mcmc->num_move_clock_r]         = .0; */
  /* mcmc->move_weight[mcmc->num_move_tree_height]     = .0; */
  /* mcmc->move_weight[mcmc->num_move_subtree_height]  = .0; */
  /* mcmc->move_weight[mcmc->num_move_nu]              = .0; */
  /* mcmc->move_weight[mcmc->num_move_kappa]           = .0;    */
  /* mcmc->move_weight[mcmc->num_move_tree_rates]     = 1.0;    */
 
  sum = 0.0;
  For(i,mcmc->n_moves) sum += mcmc->move_weight[i];
  For(i,mcmc->n_moves) mcmc->move_weight[i] /= sum;
  for(i=1;i<mcmc->n_moves;i++) mcmc->move_weight[i] += mcmc->move_weight[i-1];
}

/*********************************************************/

void MCMC_Initialize_Param_Val(t_mcmc *mcmc, t_tree *tree)
{
  int i;

  mcmc->old_param_val[mcmc->num_move_nu]          = tree->rates->nu;
  mcmc->old_param_val[mcmc->num_move_clock_r]     = tree->rates->clock_r;
  mcmc->old_param_val[mcmc->num_move_tree_height] = tree->rates->nd_t[tree->n_root->num];
  mcmc->old_param_val[mcmc->num_move_kappa]       = tree->mod->kappa;

  For(i,2*tree->n_otu-2)
    mcmc->old_param_val[mcmc->num_move_br_r+i] = tree->rates->br_r[i];
  
  For(i,tree->n_otu-1)
    mcmc->old_param_val[mcmc->num_move_nd_t+i] = tree->rates->nd_t[tree->n_otu+i];

  For(i,2*tree->n_otu-1)
    mcmc->old_param_val[mcmc->num_move_nd_r+i] = tree->rates->nd_r[i];

  For(i,mcmc->n_moves) tree->mcmc->new_param_val[i] = tree->mcmc->old_param_val[i];
}

/*********************************************************/

void MCMC_Copy_To_New_Param_Val(t_mcmc *mcmc, t_tree *tree)
{
  int i;

  mcmc->new_param_val[mcmc->num_move_nu]          = tree->rates->nu;
  mcmc->new_param_val[mcmc->num_move_clock_r]     = tree->rates->clock_r;
  mcmc->new_param_val[mcmc->num_move_tree_height] = tree->rates->nd_t[tree->n_root->num];
  mcmc->new_param_val[mcmc->num_move_kappa]       = tree->mod->kappa;

  For(i,2*tree->n_otu-2)
    mcmc->new_param_val[mcmc->num_move_br_r+i] = tree->rates->br_r[i];
  
  For(i,tree->n_otu-1)
    mcmc->new_param_val[mcmc->num_move_nd_t+i] = tree->rates->nd_t[tree->n_otu+i];

  For(i,2*tree->n_otu-1)
    mcmc->new_param_val[mcmc->num_move_nd_r+i] = tree->rates->nd_r[i];
  
}

/*********************************************************/

void MCMC_Slice_One_Rate(t_node *a, t_node *d, int traversal, t_tree *tree)
{
  phydbl L,R; /* Left and Right limits of the slice */
  phydbl w; /* window width */
  phydbl u;
  phydbl x0,x1;
  phydbl logy;
  t_edge *b;
  int i;



  b = NULL;
  if(a == tree->n_root) b = tree->e_root;
  else For(i,3) if(d->v[i] == a) { b = d->b[i]; break; }
  
  w = 0.05;
  /* w = 10.; */

  x0 = tree->rates->br_r[d->num];
  logy = tree->c_lnL+tree->rates->c_lnL - Rexp(1.);
  
  u = Uni();

  L = x0 - w*u;  
  R = L + w;
  
  do
    {
      tree->rates->br_r[d->num] = L;
      tree->rates->br_do_updt[d->num] = YES;
      RATES_Update_Cur_Bl(tree);
      Lk_At_Given_Edge(b,tree);
      RATES_Lk_Rates(tree);
      if(L < tree->rates->min_rate) { L = tree->rates->min_rate - w; break;}
      L = L - w;
    }
  while(tree->c_lnL + tree->rates->c_lnL > logy);
  L = L + w;


  do
    {
      tree->rates->br_r[d->num] = R;
      tree->rates->br_do_updt[d->num] = YES;
      RATES_Update_Cur_Bl(tree);
      Lk_At_Given_Edge(b,tree);
      RATES_Lk_Rates(tree);
      if(R > tree->rates->max_rate) { R = tree->rates->max_rate + w; break;}
      R = R + w;
    }
  while(tree->c_lnL + tree->rates->c_lnL > logy);
  R = R - w;


  for(;;)
    {
      u = Uni();
      x1 = L + u*(R-L);

      tree->rates->br_r[d->num] = x1;
      tree->rates->br_do_updt[d->num] = YES;
      RATES_Update_Cur_Bl(tree);
      Lk_At_Given_Edge(b,tree);
      RATES_Lk_Rates(tree);
      
      if(tree->c_lnL + tree->rates->c_lnL > logy) break;
      
      if(x1 < x0) L = x1;
      else        R = x1;
    }


  if(traversal == YES)
    {
      if(d->tax == YES) return;
      else
	{
	  For(i,3)
	    if(d->v[i] != a && d->b[i] != tree->e_root)
	      {
		if(tree->io->lk_approx == EXACT && tree->mcmc->use_data) Update_P_Lk(tree,d->b[i],d);
		/* if(tree->io->lk_approx == EXACT && tree->mcmc->use_data) { tree->both_sides = YES; Lk(tree); } */
		MCMC_Slice_One_Rate(d,d->v[i],YES,tree);
	      }
	}
      if(tree->io->lk_approx == EXACT && tree->mcmc->use_data) Update_P_Lk(tree,b,d);
      /* if(tree->io->lk_approx == EXACT && tree->mcmc->use_data) { tree->both_sides = YES; Lk(tree); } */
    }
}

/*********************************************************/
/*********************************************************/
/*********************************************************/
/*********************************************************/
/*********************************************************/
/*********************************************************/
/*********************************************************/
/*********************************************************/
