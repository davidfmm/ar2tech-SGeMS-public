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





#ifndef __Image_Process_h__
#define __Image_Process_h__

#include <GsTLAppli/geostat/common.h>
#include <GsTLAppli/geostat/geostat_algo.h>

#include <GsTL/matrix_library/tnt_lib.h>
#include <GsTL/matrix_library/tnt/cmat.h>

#include <vector>

#include <GsTLAppli/geostat/filtersim_std/is_categorical.h>
#include <GsTLAppli/geostat/filtersim_std/TI_manipulation.h>

class RGrid;

#define No_Action           0
#define ROT_Only           1
#define SCA_Only           2
#define FULL_Action      3

class GEOSTAT_DECL ImageProcess : public Geostat_algo 
{
public:
    ImageProcess();
    ~ImageProcess(){}

public:
	virtual bool initialize( const Parameters_handler* parameters, Error_messages_handler* errors );
	virtual int execute( GsTL_project* proj=0 );
	virtual std::string name() const { return "ImageProcess"; }
	static Named_interface* create_new_interface( std::string& ) { return new ImageProcess; }

private:
    void perform_no_action();
    void perform_rotation();
    void perform_scaling();
    void perform_full_action();

    bool get_Source_Data(const Parameters_handler* parameters, Error_messages_handler* error_mesgs );
    bool get_Target_Data(const Parameters_handler* parameters, Error_messages_handler* error_mesgs );
    bool get_Rotation_Data(const Parameters_handler* parameters, Error_messages_handler* error_mesgs );
    bool get_Scaling_Data(const Parameters_handler* parameters, Error_messages_handler* error_mesgs );

private:
    // source
    RGrid* source_grid_;
    GsTLGridProperty* source_property_;

    // target
    RGrid* target_grid_;
    GsTLGridProperty* target_property_;
    string target_prop_name_;

    // rotation
    vector<float> angles_;

    // scaling
    vector<float> factors_;

    // the TI centers
    vector<float> source_center_;
    vector<float> target_center_;

    // action type
    int action_type_;
};


#endif  // __Image_Process_h__
