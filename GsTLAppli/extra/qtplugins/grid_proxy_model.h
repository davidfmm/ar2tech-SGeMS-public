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



/*
 * project_proxy_model.h
 *
 *  Created on: Jun 8, 2011
 *      Author: aboucher
 */

#ifndef GRID_PROXY_MODEL_H_
#define GRID_PROXY_MODEL_H_

#include <GsTLAppli/utils/manager.h>
#include <GsTLAppli/appli/root_model.h>
#include <GsTLAppli/extra/qtplugins/common.h>

#include <QModelIndex>
#include <QVariant>
#include <QStringList>
#include <QAbstractProxyModel>


class QTPLUGINS_DECL No_selection_item : public GsTL_object_item {

public :
	No_selection_item();
	virtual ~No_selection_item();


};


class QTPLUGINS_DECL Grid_proxy_model : public QAbstractProxyModel{

	Q_OBJECT
public:
	Grid_proxy_model(QObject *parent = 0);
	virtual ~Grid_proxy_model();


    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    virtual QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &child) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

    virtual QModelIndex	mapFromSource ( const QModelIndex & sourceIndex ) const;
    virtual QModelIndex	mapToSource ( const QModelIndex & proxyIndex ) const;

//	virtual QStringList mimeTypes() const;
//	virtual QMimeData *mimeData(const QModelIndexList &indexes) const;

protected slots :
	virtual void begin_insert_proxy_rows(const QModelIndex & source_parent, int start, int end);
	virtual void begin_remove_proxy_rows(const QModelIndex & source_parent, int start, int end);
	virtual void end_insert_proxy_rows(const QModelIndex & source_parent, int start, int end);
	virtual void end_remove_proxy_rows(const QModelIndex & source_parent, int start, int end);

protected :

	Manager* grid_manager_;
  Root_model* model_;

protected :

  QStringList grid_names_;
  QStringList grid_types_;

};


class QTPLUGINS_DECL Grid_proxy_model_no_selection : public Grid_proxy_model{

	Q_OBJECT
public:
	Grid_proxy_model_no_selection(QObject *parent = 0);
	virtual ~Grid_proxy_model_no_selection();


//    virtual QVariant data(const QModelIndex &index, int role) const;
//    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
//    virtual QVariant headerData(int section, Qt::Orientation orientation,
//                        int role = Qt::DisplayRole) const;
//    virtual QModelIndex index(int row, int column,
//                      const QModelIndex &parent = QModelIndex()) const;
//    virtual QModelIndex parent(const QModelIndex &child) const;
//    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
//    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

    virtual QModelIndex	mapFromSource ( const QModelIndex & sourceIndex ) const;
    virtual QModelIndex	mapToSource ( const QModelIndex & proxyIndex ) const;

//	virtual QStringList mimeTypes() const;
//	virtual QMimeData *mimeData(const QModelIndexList &indexes) const;

protected slots :
	virtual void begin_insert_proxy_rows(const QModelIndex & source_parent, int start, int end);
	virtual void begin_remove_proxy_rows(const QModelIndex & source_parent, int start, int end);
	virtual void end_insert_proxy_rows(const QModelIndex & source_parent, int start, int end);
	virtual void end_remove_proxy_rows(const QModelIndex & source_parent, int start, int end);
private :
	No_selection_item* no_item_;

};



/*
class QTPLUGINS_DECL Grid_proxy_model : public QAbstractProxyModel{

	Q_OBJECT
public:
	Grid_proxy_model(QObject *parent = 0);
	virtual ~Grid_proxy_model();


    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    virtual QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &child) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

    virtual QModelIndex	mapFromSource ( const QModelIndex & sourceIndex ) const;
    virtual QModelIndex	mapToSource ( const QModelIndex & proxyIndex ) const;

//	virtual QStringList mimeTypes() const;
//	virtual QMimeData *mimeData(const QModelIndexList &indexes) const;

private slots :
	void begin_insert_proxy_rows(const QModelIndex & source_parent, int start, int end);
	void begin_remove_proxy_rows(const QModelIndex & source_parent, int start, int end);
	void end_insert_proxy_rows(const QModelIndex & source_parent, int start, int end);
	void end_remove_proxy_rows(const QModelIndex & source_parent, int start, int end);

protected :

	Manager* root_;

};


class QTPLUGINS_DECL Grid_proxy_model_no_selection : public Grid_proxy_model{

	Q_OBJECT
public:
	Grid_proxy_model_no_selection(QObject *parent = 0);
	virtual ~Grid_proxy_model_no_selection();


//    virtual QVariant data(const QModelIndex &index, int role) const;
//    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
//    virtual QVariant headerData(int section, Qt::Orientation orientation,
//                        int role = Qt::DisplayRole) const;
    virtual QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
//    virtual QModelIndex parent(const QModelIndex &child) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
//    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

    virtual QModelIndex	mapFromSource ( const QModelIndex & sourceIndex ) const;
    virtual QModelIndex	mapToSource ( const QModelIndex & proxyIndex ) const;

//	virtual QStringList mimeTypes() const;
//	virtual QMimeData *mimeData(const QModelIndexList &indexes) const;

private slots :
	void begin_insert_proxy_rows(const QModelIndex & source_parent, int start, int end);
	void begin_remove_proxy_rows(const QModelIndex & source_parent, int start, int end);
	void end_insert_proxy_rows(const QModelIndex & source_parent, int start, int end);
	void end_remove_proxy_rows(const QModelIndex & source_parent, int start, int end);
private :
	No_selection_item* no_item_;

};
*/
#endif /* PROJECT_PROXY_MODEL_H_ */
