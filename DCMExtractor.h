#pragma once

#include <QtCore>
#include <stack>
#include <dcmtk/dcmdata/dcfilefo.h>
#include "ui_DCMExtractor.h"
#include <QTreeWidgetItem>


class DCMExtractor : public QMainWindow
{
	Q_OBJECT
public:
	DCMExtractor(QWidget* t_parent = Q_NULLPTR);

private slots:
	void on_actionOpen_triggered();
	

private:
	Ui::DCMExtractorClass m_ui = {};
	std::stack<QTreeWidgetItem* > m_lastParents = {}; //Stack cu ultimii parinti din QTree
	unsigned long m_lastDepth = {};
	int m_lineCounter = {};
	QString m_path = {};

	void printElement(DcmElement* t_children, 
		const bool& t_isChild, const unsigned long& t_currentDepth);
	QTreeWidgetItem* createItem(DcmElement* t_element) const;
	void initData();
	void appendEndTag(QTreeWidgetItem* t_parent) const;
	void appendEndSQ(QTreeWidgetItem* t_parent, const bool& t_isRoot) const;
};
