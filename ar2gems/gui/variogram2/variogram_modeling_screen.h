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
** This file is part of the "gui" module of the Geostatistical Earth
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

#ifndef __GSTLAPPLI_GUI_VARIOGRAM_MODELING_SCREEN_H__
#define __GSTLAPPLI_GUI_VARIOGRAM_MODELING_SCREEN_H__

#include <gui/common.h>
#include <math/gstlpoint.h>
#include <math/gstlvector.h>
#include <math/discrete_function.h>
#include <qwt/qwt_plot.h>

#include <GsTL/geometry/covariance.h>

#include <qapplication.h>
#include <qpixmap.h>
#include <qwidget.h>
#include <qstringlist.h>
//Added by qt3to4:
#include <QVBoxLayout>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>

#include <vector>

class QSplitter;
class QScrollArea;
class QWorkspace;

class Variogram_controls;
class Variog_plot;

class QWorkspace;

class GUI_DECL Variogram_modeling_screen : public QWidget {

  Q_OBJECT
	
  typedef Variogram_function_adaptor< Covariance<GsTLPoint> > Variogram_;

public:    
  Variogram_modeling_screen( QWidget *parent=0, const char *name=0, Qt::WFlags fl=0 );
  virtual ~Variogram_modeling_screen();
    
  void run( const std::vector<Discrete_function>& df,
    	      const Variogram_ *model,
	          Covariance<GsTLPoint> *model_cov,
	          const std::vector< GsTLVector<double> >& angle,
            const std::vector< std::vector<int> >& pairs,
            const QStringList& parameters_list );

public slots:
  void save_experimental_variograms();
  void save_plots_as_images();
  void show_plots_settings_dialog();

private:
  void init();
  void delete_plots();
     
private:
  std::vector<Discrete_function> experimental_variograms_;
  QStringList parameters_list_;
  std::vector< GsTLVector<double> > directions_;
  std::vector< std::vector<int> > pairs_;

protected:
  QSplitter* splitter_;
  QWorkspace* display_panel_;
   
  Variogram_controls* controls_;

//  std::vector<Variog_plot*> plots_;    
  std::vector<QwtPlot*> plots_;    
  QScrollArea* scroll_;
   
 };


#endif
	
