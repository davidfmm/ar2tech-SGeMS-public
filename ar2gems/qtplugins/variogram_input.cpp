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

** This file is part of the "extra" module of the Geostatistical Earth

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



#include <qtplugins/variogram_input.h>

#include <qtplugins/gstl_table.h>

//#include <widget_accessor/qt_accessors.h>

#include <utils/gstl_messages.h>



#include <qstring.h>

#include <qcombobox.h>

#include <qlineedit.h>

#include <QFrame>

#include <qfont.h>

#include <qlayout.h>

#include <qlabel.h>

#include <qspinbox.h>



#include <QGroupBox>

#include <qfile.h>

#include <QFileDialog>

#include <QTextStream>

#include <qpushbutton.h>

#include <qdom.h>

//Added by qt3to4:

#include <QHBoxLayout>

#include <QVBoxLayout>



VariogramInput::VariogramInput( QWidget* parent, const char* name )

  : QWidget( parent ) {



  if (name)

    setObjectName(name);



  setupUi(this);

  

  structures_frame_ = new QFrame( structures_main_box_);
  structures_main_box_->layout()->addWidget(structures_frame_);

  QVBoxLayout * layout = new QVBoxLayout;

  structures_frame_->setLayout(layout);


  layout->setSpacing( 8 );

  layout->setMargin( 1 );





  // Make one structure by default

  Line_separator* separator1 = 

    new Line_separator( "Structure 1", structures_frame_);

  Variogram_structure_input* struct1 = 

    new Variogram_structure_input(structures_frame_ );



  layout->addWidget(separator1);

  layout->addWidget(struct1);

  

  

  structures_.push_back( std::make_pair( separator1, struct1 ) );





  QObject::connect( structures_count_, SIGNAL( valueChanged( int ) ),

		    this, SLOT( update_structures_count( int ) ) );

  QObject::connect( load_button_, SIGNAL( clicked() ),

		    this, SLOT( load_model() ) );

}





void VariogramInput::update_structures_count(int val ) {

  int current_count = structures_.size();

  if( current_count == val ) return;



  if( current_count < val ) {

    // add one (or more) new structure(s)

    for( int i=0; i < val - current_count; i ++ ) {

      

      QString count_str;

      count_str.setNum( current_count + i + 1 );



      Line_separator* separator =

      	new Line_separator( "Structure " + count_str);

      Variogram_structure_input* structure = 

	      new Variogram_structure_input();

      

      structures_frame_->layout()->addWidget(separator);

      structures_frame_->layout()->addWidget(structure);

      

      structure->show();

      separator->show();

      structures_.push_back( std::make_pair( separator, structure ) );

    }

  }

  else {

    // remove one (or more) structure(s)

    for( int j = current_count-1; j >= val  ; j -- ) {

      delete structures_[ j ].first;

      delete structures_[ j ].second;

      structures_.pop_back(); 

      

    }

  }



  structures_count_->setValue( val );

}







float VariogramInput::nugget() const {

  QString val = nugget_effect_edit_->text();

  return val.toFloat();

}



void VariogramInput::set_nugget( float n ) {

  QString val;

  val.setNum( n );

  nugget_effect_edit_->setText( val );

}



int VariogramInput::structures_count() const {

  return structures_.size();

}



const Variogram_structure_input* VariogramInput::structure( int id ) const {

  if( id >= int( structures_.size() ) ) return 0;



  return structures_[id].second;

}



Variogram_structure_input* VariogramInput::structure( int id ) {

  if( id >= int( structures_.size() ) ) return 0;



  return structures_[id].second;

}



void VariogramInput::load_model() {

  QString filename = 

    QFileDialog::getOpenFileName( this, "Load Variogram Model" );

  

  if( filename == QString::null ) return;



  // Open the file and put the content into a string (using a stringstream)

  QFile file( filename );

  if ( !file.open( QIODevice::ReadOnly ) ) {

    QByteArray tmp = filename.toAscii();

    GsTLcerr << "Can't open file " << tmp.constData() << "\n" << gstlIO::end;

    return;

  }



  QTextStream stream( &file );

  QString qstr = stream.readAll();

  if( qstr ==QString::null ) return;

  

  // to avoid circular dependencies, we're not using the accessor anymore

  /* 

  VariogramInput_accessor accessor( this );

  accessor.set_value( qstr.latin1() );

  */



  init_values_from_xml_string( qstr );

}





/* This function is a copy of VariogramInput_accessor::set_value. It has been

* copied to avoid circular dependency issues: library GsTLwidgets would depend

* on GsTLAppli_extragui, which already depends on GsTLwidgets

*/

bool VariogramInput::init_values_from_xml_string( QString& qstr ) {

  

  // qstr is just an element of an xml file, hence can not be parsed

  // by QDomDocument. We need to add a root element.

  qstr = "<root>" + qstr + "</root>";

  QDomDocument doc;

  bool parsed = doc.setContent( qstr );

  appli_assert( parsed );



  QDomElement root_element = doc.documentElement();

  QDomElement elem = root_element.firstChild().toElement();

  

  // Get the nugget value and the number of structures

  QString val = elem.attribute( "nugget" );

  this->set_nugget( val.toFloat() );

  val = elem.attribute( "structures_count" );

  this->update_structures_count( val.toInt() );



  // work on each structure

  for( int i=0; i < this->structures_count() ; i++ ) {

    Variogram_structure_input* structure = this->structure( i );

    appli_assert( structure );

    

    // Get the node describing the structure and initialize the contribution

    // and variogram type values

    QString id;

    id.setNum( i+1 );

    QString structure_tagname = "structure_" + id;

    QDomNodeList nodes = elem.elementsByTagName( structure_tagname );

    if( nodes.count() == 0 ) {

      QByteArray tmp = structure_tagname.toLatin1();

      appli_warning( "No element called \"" << tmp.constData()  << "\"" << std::endl

		    << "Aborting" );

      return false;

    }

    QDomNode structure_node = nodes.item(0);

    appli_assert( structure_node.isElement() );

    QDomElement structure_elem = structure_node.toElement();



    val = structure_elem.attribute( "contribution" );

    structure->set_contribution( val.toFloat() );

    structure->set_variogram_type( structure_elem.attribute( "type" ) );





    // Get the ranges

    QDomNodeList ranges_node_list = structure_elem.elementsByTagName( "ranges" );

    if( ranges_node_list.count() == 0 ) {

      appli_warning( "No element called \"ranges\"" << std::endl

		    << "Aborting" );

      return false;

    }

    QDomNode ranges_node = ranges_node_list.item(0);

    appli_assert( ranges_node.isElement() );

    QDomElement ranges_elem = ranges_node.toElement();

    

    val = ranges_elem.attribute( "max" );

    structure->max_range( val.toFloat() );

    val = ranges_elem.attribute( "medium" );

    structure->medium_range( val.toFloat() );

    val = ranges_elem.attribute( "min" );

    structure->min_range( val.toFloat() );



    

    // Get the angles

    QDomNodeList angles_node_list = structure_elem.elementsByTagName( "angles" );

    if( angles_node_list.count() == 0 ) {

      appli_warning( "No element called \"angles\"" << std::endl

		    << "Aborting" );

      return false;

    }

    QDomNode angles_node = angles_node_list.item(0);

    appli_assert( angles_node.isElement() );

    QDomElement angles_elem = angles_node.toElement();

    

    val = angles_elem.attribute( "x" );

    structure->x_angle( val.toFloat() );

    val = angles_elem.attribute( "y" );

    structure->y_angle( val.toFloat() );

    val = angles_elem.attribute( "z" );

    structure->z_angle( val.toFloat() );



  }



  return true;

}





//====================================================



Variogram_structure_input::

Variogram_structure_input( QWidget* parent, const char* name ) 

  : QWidget( parent ) {



  setupUi(this);

  if (name)
	  this->setObjectName(name);

  QVBoxLayout * vbox = new QVBoxLayout;

  anisotropy_table_box_->setLayout(vbox);

  



  //anisotropy_table_ = new EllipsoidInput( anisotropy_table_box_, "table" );

  anisotropy_table_ = new EllipsoidInput(anisotropy_table_box_);

  vbox->addWidget(anisotropy_table_);

  

  //anisotropy_table_->set_max_width( 250 );



  QObject::connect( type_selector_, 

		   SIGNAL( activated( const QString& ) ),

		   SIGNAL( variogram_type_changed( const QString& ) ) );

  QObject::connect( contribution_edit_, SIGNAL( textChanged( const QString& ) ),

		   this, SLOT( contribution_change_slot( const QString& ) ) );



 

}

 

 

void Variogram_structure_input::set_ranges( float max, float medium, float min ) {

  QString val;

  anisotropy_table_->set_ranges( max, medium, min );



  emit anisotropy_value_changed();

}



void Variogram_structure_input::

ranges( float& max, float& medium, float& min ) const {

  double r1, r2, r3;

  anisotropy_table_->ranges( r1, r2, r3 );

  max = r1;

  medium = r2;

  min = r3;

}





void Variogram_structure_input::max_range( float a ) {

  anisotropy_table_->set_max_range( a );

  emit anisotropy_value_changed();

}





float Variogram_structure_input::max_range() const {

  return anisotropy_table_->max_range();

}



void Variogram_structure_input::medium_range( float a ) {

  anisotropy_table_->set_med_range( a );

  emit anisotropy_value_changed();

}



float Variogram_structure_input::medium_range() const {

  return anisotropy_table_->med_range();

}





void Variogram_structure_input::min_range( float a ) {

  anisotropy_table_->set_min_range( a );

  emit anisotropy_value_changed();

}



float Variogram_structure_input::min_range() const {

  return anisotropy_table_->min_range();

}









void Variogram_structure_input::set_angles( float max, float medium, float min ) {

  anisotropy_table_->set_angles( max, medium, min );

  emit anisotropy_value_changed();

}



void Variogram_structure_input::

angles( float& max,	float& medium, float& min ) const {

  double a1, a2, a3;

  anisotropy_table_->angles( a1, a2, a3 );

  max = a1;

  medium = a2;

  min = a3;

}



void Variogram_structure_input::x_angle( float a ) {

  anisotropy_table_->set_azimuth( a );

  emit anisotropy_value_changed();

}



float Variogram_structure_input::x_angle() const {

  return anisotropy_table_->azimuth();

}





void Variogram_structure_input::y_angle( float a ) {

  anisotropy_table_->set_dip( a );

  emit anisotropy_value_changed();

}



float Variogram_structure_input::y_angle() const {

  return anisotropy_table_->dip();

}





void Variogram_structure_input::z_angle( float a ) {

  anisotropy_table_->set_rake( a );

  emit anisotropy_value_changed();

}



float Variogram_structure_input::z_angle() const {

  return anisotropy_table_->rake();

}

  



void Variogram_structure_input::contribution_change_slot( const QString& val ) {

  float c = val.toFloat();

  emit contribution_changed( c );

}





void Variogram_structure_input::set_contribution( float c ) {

  QString val;

  val.setNum( c );

  contribution_edit_->setText( val );

}



float Variogram_structure_input::contribution() const {

  QString val = contribution_edit_->text();

  return val.toFloat();

}





QString Variogram_structure_input::variogram_type() const {

  return type_selector_->currentText(); 

}





void Variogram_structure_input::set_variogram_type( const QString& type ) {

  type_selector_->setItemText( type_selector_->currentIndex(), type );

} 





//==============================================================



Line_separator::Line_separator( const QString& label, 

				QWidget* parent, const char* name ) 

  : QWidget( parent ) {

  
  if (name)
	  setObjectName(name);

  QHBoxLayout* layout = new QHBoxLayout( this );



  QLabel* separator_label = new QLabel( label, this );

  QFont font;

  font.setBold( true );

  //  separator_label->setFont( font );

  QSizePolicy label_policy( QSizePolicy::Fixed, QSizePolicy::Fixed );

  separator_label->setSizePolicy( label_policy );

  

  QFrame* line = new QFrame( this);

  line->setFrameShape( QFrame::Box );

  line->setFrameShadow( QFrame::Raised );

  QSizePolicy line_policy( QSizePolicy::Expanding, QSizePolicy::Fixed );

  line->setSizePolicy( line_policy );

  

  layout->addWidget( separator_label );

  layout->addWidget( line );

  

}

