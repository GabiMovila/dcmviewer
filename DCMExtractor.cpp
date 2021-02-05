#include "DCMExtractor.h"
#include <dcmtk/dcmdata/dcdeftag.h>
#include <QFileDialog> 
#include <QMessageBox>


constexpr auto max_characters = 100;
constexpr auto endTagItem = "(fffe,e00d)";
constexpr auto endTagSq = "(fffe,e0dd)";


DCMExtractor::DCMExtractor(QWidget* t_parent)
	: QMainWindow(t_parent)
{
	m_ui.setupUi(this);
}

void DCMExtractor::initData()
{
	// Luam path-ul din file->open file
	const auto& path = QFileDialog::getOpenFileName(this);

	if (path.isEmpty())
	{
		QMessageBox msgBox(QMessageBox::Warning,
			"Error", "Cannot load file!");
		msgBox.exec();
		return;
	}
	m_path = path;
	m_ui.treeWidget->clear();
	// Setam labelurile ce le punem in Qtree 
	m_ui.treeWidget->setColumnCount(6);
	const QStringList labels = { "Tag ID ",  "VR" , "VM" , "Length", "Description", "Value" };
	m_ui.treeWidget->setHeaderLabels(labels);

	m_lastDepth = 0;   // Folosit la construirea Qtree-ului pentru a sti unde sa punem copiii, variabila memoreaza depthul ultimului element
	m_lineCounter = 0; // Folosit la numararea liniilor din fisierele folosite pentru validare 

	//ofstream OutputFile("ObtainedFile.txt"); // sa nu tot dea append la fisier dupa fiecare rulare
	//OutputFile.close();

	//fileformat->loadAllDataIntoMemory(); // Load in memorie
}

void DCMExtractor::on_actionOpen_triggered()
{
	initData(); // Initializam unele date

	DcmStack stack;
	const auto& fileformat = std::make_unique<DcmFileFormat>();
	if (fileformat->loadFile(m_path.toStdString().data()).good())
	{
		OFCondition status = fileformat->nextObject(stack, OFTrue);

		while (status.good()) // Loop-uim prin stack
		{
			const auto& object = stack.top();
			const auto& element = static_cast<DcmElement*>(object); //II facem cast din obiect in element
			const bool isNested = element->isNested();

			printElement(element, isNested, stack.card()); //Printat node-ul pe ecran in tree widget

			//Printat node-ul in fisier pentru testare. Uncomment daca vrei sa faci si partea de testare (timp de executie relativ mare)
				//PrintTreeToXML(element, stack.card()); 	

			status = fileformat->nextObject(stack, OFTrue); // Trece la urmatorul element
		}

		m_ui.treeWidget->expandAll();
		m_ui.treeWidget->setColumnWidth(0, 300);
	}
	else
	{
		QMessageBox msgBox(QMessageBox::Warning,
			"Error", "Cannot load file!");
		msgBox.exec();
	}
}

void DCMExtractor::printElement(DcmElement* t_children, const bool& t_isChild, const unsigned long& t_currentDepth) // t_isChild = 1 : il pui ca si copil, t_isChild = 0 il pui direct root
{
	// Intra aici and o ajuns la o lungime maxima si trebuie sa inceapa sa dea pop la parinti
	while (t_currentDepth < m_lastDepth) //Dam pop la parinti pana ajungem la lungimea buna
	{
		if (!strcmp(m_lastParents.top()->text(1).toStdString().c_str(), "na")) // Daca sunt iteme neinchise printeaza mai intai finalul de item
		{
			m_lastParents.pop();
			m_lastDepth--;
			appendEndTag(m_lastParents.top());
			continue;
		}

		if (!strcmp(m_lastParents.top()->text(1).toStdString().c_str(), "SQ")) // Daca sunt secvente neinchise printeaza mai intai finalul de secvente
		{
			if (!m_lastParents.top()->parent()) // Daca e secventa root (nenestuita atunci inchidem secventa in top level item -> bool-ul e true)
				appendEndSQ(m_lastParents.top(), true);
			else
			{
				m_lastParents.pop();
				m_lastDepth--;
				appendEndSQ(m_lastParents.top(), false); // Daca e secventa nestuita atunci inchidem secventa la nivelul corespunzator -> bool-ul e true)
				continue;
			}

		}

		m_lastParents.pop();
		m_lastDepth--;
	}

	if (t_isChild) // In functie de bool-ul ala vedem daca sa-l punem top level item sau child
	{
		QTreeWidgetItem* itm = createItem(t_children);
		if (!t_children->isLeaf())
		{
			// Daca nodul asta e tree il adaug ca si copil la ultimul parinte apoi il pun pe el ca fiind ultimul parinte
			m_lastParents.top()->addChild(itm);
			m_lastParents.push(itm);
		}
		else // Daca e leaf
		{
			// Luam ultimul parinte din stack si adaugam elementul asta ca si copil al lui
			m_lastParents.top()->addChild(itm);
		}
	}

	else
	{
		// ====== Verificam daca e item ca top level atunci il sarim =======

		const DcmVR vrrr = t_children->getVR();  // Get VR
		const QString vr = vrrr.getVRName();
		if (!strcmp(vr.toStdString().c_str(), "na"))
			return;

		// ===== Daca nu e un itm drept top level adaugam tag-ul respectiv in top level ======
		QTreeWidgetItem* itm = createItem(t_children);
		m_ui.treeWidget->addTopLevelItem(itm);
		m_lastParents.push(itm);
	}
	m_lastDepth = t_currentDepth;	//Salvam depth-ul actual ca fiind m_lastDepth
}

QTreeWidgetItem* DCMExtractor::createItem(DcmElement* t_element) const
{
	const QString tag = t_element->getTag().toString().c_str();  // Get Tag
	const QString vm = std::to_string(t_element->getVM()).c_str(); // Get VM
	const QString length = std::to_string(t_element->getLength()).c_str(); //Get Length
	const DcmVR vrrr = t_element->getVR();  // Get VR
	const QString vr = vrrr.getVRName();  // Get VR
	DcmTag tagname = t_element->getTag(); //  Name
	const QString name = tagname.getTagName(); //  Name
	std::string value = {};
	
	if (t_element->getTag() != DCM_ItemTag) // Daca nu-i tag ii luam valoarea in limita a maxCharcters definit la inceputul programului
	{
		OFString buffer={};
		t_element->getOFString(buffer,0);
		value.append(buffer.data(), buffer.size() < max_characters ? buffer.size() : max_characters);
		std::replace(value.begin(), value.end(), '\n',' ');
	}
	
	//value.replace(max_characters, 2000000, ""); // Sterge tot de la pozitia max_characters incolo
	//value.remove('\n'); // sterge new line urile
	const auto& itm = new QTreeWidgetItem();
	int column = 0;
	itm->setText(column++, tag);
	itm->setText(column++, vr);
	itm->setText(column++, vm);
	itm->setText(column++, length);
	itm->setText(column++, name);
	itm->setText(column, QString::fromStdString(value));
	return itm;
}

void DCMExtractor::appendEndTag(QTreeWidgetItem* t_parent) const
{
	const auto& itm = new QTreeWidgetItem();
	itm->setText(0, endTagItem);
	t_parent->addChild(itm);
}

void DCMExtractor::appendEndSQ(QTreeWidgetItem* t_parent, const bool& t_isRoot) const
// In functie de bool-ul asta incheia secventa ori la top level, ori la levelul necesar
{
	if (t_isRoot)
	{
		const auto& itm2 = new QTreeWidgetItem(m_ui.treeWidget);
		itm2->setText(0, endTagSq);
		m_ui.treeWidget->addTopLevelItem(itm2);
	}
	else
	{
		const auto& itm = new QTreeWidgetItem();
		itm->setText(0, endTagSq);
		t_parent->addChild(itm);
	}

}