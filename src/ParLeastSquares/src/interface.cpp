#include <ParLeastSquares>

[[nodiscard]]
Eigen::VectorXd least_squares_analytical(
    Eigen::MatrixXd& S_mat,
    Eigen::MatrixXd& R_back_mat,
    Eigen::MatrixXd& P_mat, 
    Eigen::VectorXd& Keq_constant,
    Eigen::VectorXd& E_Regulation,
    Eigen::VectorXd& log_fcounts,
    Eigen::VectorXd& log_vcounts,
    const int maxfev,
    const double xtol)
{
  LMFunctorAnalytical functor(
      S_mat,
      R_back_mat,
      P_mat,
      Keq_constant,
      E_Regulation,
      log_fcounts);
  Eigen::LevenbergMarquardt<LMFunctorAnalytical> lm(functor);

  lm.parameters.maxfev = maxfev;
  lm.parameters.xtol = xtol;
  lm.minimize(log_vcounts);

  return log_vcounts;
}

  [[nodiscard]]
Eigen::VectorXd least_squares_numerical(
    Eigen::MatrixXd& S_mat,
    Eigen::MatrixXd& R_back_mat,
    Eigen::MatrixXd& P_mat, 
    Eigen::VectorXd& Keq_constant,
    Eigen::VectorXd& E_Regulation,
    Eigen::VectorXd& log_fcounts,
    Eigen::VectorXd& log_vcounts,
    const int maxfev,
    const double xtol)
{
  LMFunctor functor(
      S_mat,
      R_back_mat,
      P_mat,
      Keq_constant,
      E_Regulation,
      log_fcounts);
  Eigen::NumericalDiff<LMFunctor> numDiff(functor);
  Eigen::LevenbergMarquardt<Eigen::NumericalDiff<LMFunctor>, double> lm(numDiff);

  lm.parameters.maxfev = maxfev;
  lm.parameters.xtol = xtol;
  
  //std::cout<<"pre minimize point " << log_vcounts << std::endl;
  lm.minimize(log_vcounts); //This log_vcounts is changing the minimize point.


  return log_vcounts;
}

/*
 * Driver function for calculating the least squares with the
 * Levenberg-Marquardt method
 */
  [[nodiscard]]
Eigen::VectorXd least_squares(
    Eigen::MatrixXd& S_mat,
    Eigen::MatrixXd& R_back_mat,
    Eigen::MatrixXd& P_mat, 
    Eigen::VectorXd& Keq_constant,
    Eigen::VectorXd& E_Regulation,
    Eigen::VectorXd& log_fcounts,
    Eigen::VectorXd& log_vcounts,
    DiffType dt,
    const int maxfev,
    const double xtol)
{
 // std::cout << "maxfev: " << maxfev
  //  << "\nxtol: " << xtol
  //  << "\n";

  switch (dt)
  {
    case DiffType::Numerical:
      return least_squares_numerical(
          S_mat,
          R_back_mat,
          P_mat, 
          Keq_constant,
          E_Regulation,
          log_fcounts,
          log_vcounts,
          maxfev,
          xtol);
    case DiffType::Analytical:
      return least_squares_analytical(
          S_mat,
          R_back_mat,
          P_mat, 
          Keq_constant,
          E_Regulation,
          log_fcounts,
          log_vcounts,
          maxfev,
          xtol);
  };
  assert (false && "Must pass valid differentiation method.");
}
