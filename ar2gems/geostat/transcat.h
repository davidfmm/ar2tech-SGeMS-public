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
** Author: Jianbing Wu
**
** Copyright (C) 2002-2004 The Board of Trustees of the Leland Stanford Junior
**   University
** All rights reserved.
**
** This file is part of the "geostat" module of the Stanford Geostatistical 
** Earth Modeling Software (SGEMS)
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


#ifndef __transcat_h__
#define __transcat_h__

#include <geostat/common.h>
#include <geostat/geostat_algo.h>
#include <utils/gstl_types.h>
#include <grid/geovalue.h>
#include <grid/property_copier.h>

#include <GsTL/utils/smartptr.h>
#include <GsTL/matrix_library/tnt_lib.h>
#include <GsTL/matrix_library/tnt/cmat.h>

#include <vector>
#include <string>

#include <geostat/filtersim_std/is_categorical.h>
#include <geostat/filtersim_std/filters.h>

class Strati_grid;
class Grid_template;
class Geostat_grid;
class RGrid;
class Neighborhood;
class Grid_continuous_property;

using namespace std;
using namespace geostat_utils;


class GEOSTAT_DECL transcat : public Geostat_algo 
{
public:
    transcat();
    ~transcat();

public:
	virtual bool initialize( const Parameters_handler* parameters, Error_messages_handler* errors );
	virtual int execute( GsTL_project* proj=0 );
	virtual std::string name() const { return "transcat"; }
	static Named_interface* create_new_interface( std::string& ) { return new transcat; }

private:
    bool get_target_prop(const Parameters_handler* parameters, Error_messages_handler* error_mesgs);
    bool get_source_data(const Parameters_handler* parameters, Error_messages_handler* error_mesgs );
    bool get_condition_data( const Parameters_handler* parameters, Error_messages_handler* error_mesgs );
    bool get_moving_window( const Parameters_handler* parameters, Error_messages_handler* error_mesgs );

    bool get_filters(const Parameters_handler* parameters, Error_messages_handler* error_mesgs);
    void create_default_filter();
    bool create_user_define_filter( const Parameters_handler* parameters, Error_messages_handler* error_mesgs );
    bool valid_filter_file( const Parameters_handler* parameters, Error_messages_handler* error_mesgs,
                                    int nxdt, int nydt, int nzdt, string filter_file_name );

    void calculate_category_proportions( Grid_continuous_property* prop );

private:
    // source
    RGrid* source_grid_;
    Grid_continuous_property* source_property_;
    string source_property_name_;
    string output_suffix_;

    // for local conditioning
    int use_local_condition_;
	Geostat_grid* harddata_grid_;
	Grid_continuous_property* harddata_property_;
	SmartPtr<Property_copier> property_copier_;
    float locl_cond_factor_;
    string cond_prop_name_;

	int nb_facies_;
    int nb_iterations_;
    vector<float> target_pdf_;
    vector<float> target_pdf_factor_;
    vector<float> current_prop_;
    vector<float> current_prop_factor_;

	// moving windows dimensions
	int nxdt_;
	int nydt_;
	int nzdt_;
    int nxyzdt_;

    // filter definitions
    int use_default_filter_;
	Filter* my_filter_;
    vector<float> filter_weight_;
    Grid_template* window_geom_;
};


#endif  
