/* -----------------------------------------------------------------------------
** Copyright (c) 2012 Advanced Resources and Risk Technology, LLC
** All rights reserved.
**
** This file is part of Advanced Resources and Risk Technology, LLC (AR2TECH) 
** version of the open source software sgems.  It is a derivative work by 
** AR2TECH (THE LICENSOR) based on the x-free license granted in the original 
** version of the software (see notice below) and now sublicensed such that it 
** cannot be distributed or modified without the explicit and written permission 
** of AR2TECH.
**
** Only AR2TECH can modify, alter or revoke the licensing terms for this 
** file/software.
**
** This file cannot be modified or distributed without the explicit and written 
** consent of AR2TECH.
**
** Contact Dr. Alex Boucher (aboucher@ar2tech.com) for any questions regarding
** the licensing of this file/software
**
** The open-source version of sgems can be downloaded at 
** sourceforge.net/projects/sgems.
** ----------------------------------------------------------------------------*/



/**********************************************************************
** Author: Nicolas Remy
** Copyright (C) 2002-2004 The Board of Trustees of the Leland Stanford Junior
**   University
** All rights reserved.
**
** This file is part of the "math" module of the Geostatistical Earth
** Modeling Software (GEMS)
**
** This file may be distributed and/or modified under the terms of the 
** license defined by the Stanford Center for Reservoir Forecasting and 
** appearing in the file LICENSE.XFREE included in the packaging of this file.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.gnu.org/copyleft/gpl.html for GPL licensing information.
**
** Contact the Stanford Center for Reservoir Forecasting, Stanford University
** if any conditions of this licensing are not clear to you.
**
**********************************************************************/

#ifndef __GSLTAPPLI_MATH_CORRELATION_MEASURE_COMPUTER_H__
#define __GSLTAPPLI_MATH_CORRELATION_MEASURE_COMPUTER_H__

#include <math/common.h>

#include <vector>
#include <utility>
#include <string>
#include <map>



class MATH_DECL Correlation_measure_computer {
public:
  typedef std::vector< std::vector< std::pair<float,float> > > PairMatrix;
  typedef std::vector< std::pair<float,float> >  PairVector;

  enum MeasureType{ semivariogram=0, covariance, correlogram,
	                  general_relative_semivariogram, logarithmic_semivariogram,
	                  semimadogram, indicator};
  
  static const double NaN;

  Correlation_measure_computer();

  /** Computes the correlation measures for the pairs in \c matrix.
  */
  std::vector<double> operator() ( const PairMatrix& matrix, 
                                   const std::string& measure_type, double param );

  /** Computes the correlation measures for the pairs in \c pairs.
  */
  double operator() ( const PairVector& pairs, 
                      const std::string& measure_type, double param );

protected:
  void compute_variogram( std::vector<double>& result,
                          const PairMatrix& matrix );
  void compute_covariance( std::vector<double>& result,
                          const PairMatrix& matrix );
  void compute_correlogram( std::vector<double>& result,
                          const PairMatrix& matrix );
  void compute_general_variogram( std::vector<double>& result,
                          const PairMatrix& matrix );
  void compute_log_variogram( std::vector<double>& result,
                          const PairMatrix& matrix );
  void compute_madogram( std::vector<double>& result,
                          const PairMatrix& matrix );
  void compute_indicator_variogram( std::vector<double>& result,
                                    const PairMatrix& matrix,
                                    double param );

protected:
  double compute_variogram( const PairVector& pairs );
  double compute_covariance( const PairVector& pairs );
  double compute_correlogram( const PairVector& pairs );
  double compute_general_variogram( const PairVector& pairs );
  double compute_log_variogram( const PairVector& pairs );
  double compute_madogram( const PairVector& pairs );
  double compute_indicator_variogram( const PairVector& pairs,
                                      double param );


  std::pair<float,float> mean( const std::vector< std::pair<float,float> >& array );


protected:
  std::map< std::string, MeasureType > model_map_;

};

#endif