#include "Validare.h"

#include <QMessageBox>

#include "DCMExtractor.h"

//void DCMExtractor::validate(const QString& path)
//{
//
//	const auto& file = std::make_unique<DcmFileFormat>();
//	file->loadFile(path.toStdString().data());
//	ofstream outfile("OriginalFile.txt");
//
//	file->print(outfile);  	// printam toate datele originale in OriginalFile.Txt
//	outfile.close();
//	formatFile(); //Formatam fisierul scos de DCMTK (Scoatem toate liniile irelevante care nu contin taguri)
//
//	ifstream fisierOriginal("OriginalFile.txt");
//	ifstream fisierObtinut("ObtainedFile.txt");
//
//	string OriginalLine;
//	string ObtainedLine;
//	bool validareFisiere = true; //Folosit pentru a vedea daca sunt la fel fisierele. Daca o linie difera atunci acestea nu sunt la fel
//	
//	// =================================== COMPARAM LINIILE CARACTER CU CARACTER SA VEDEM DACA-S LA FEL =========================================
//	while (getline(fisierOriginal, OriginalLine))
//	{
//		getline(fisierObtinut, ObtainedLine);
//		for (int i = 0; i < OriginalLine.length(); i++)
//		{
//			if (OriginalLine[i] != ObtainedLine[i])
//			{
//				cout << "Linia " << m_lineCounter << " NU SUNT LA FEL: " << ObtainedLine[i] << " " << OriginalLine[i] << endl;
//				validareFisiere = false;
//				break;
//			}
//
//			if (OriginalLine[i] == ')')
//				break;
//		}
//		m_lineCounter++;
//	}
//	
//	if (validareFisiere) //Depinzand de valoarea variabilei vedem daca fisierele sunt la fel sau nu
//		QMessageBox::warning(this, "Validare", "Fisiere identice");
//	else
//		QMessageBox::warning(this, "Validare", "Fisiere diferite");
//	
//	fisierObtinut.close();
//	fisierOriginal.close();
//}



//void DCMExtractor::PrintTreeToXML(DcmElement* node, unsigned long depth)
//{
//
//	ofstream outFile("ObtainedFile.txt", ios_base::app);
//
//	//In original imi pune 2 space-uri idendare atunci cand ii copil
//	if (depth < 3) // Elementele pe care le pune degeaba ca-s cu un nivel mai in fata de tag-uri
//		return;
//
//	while (depth > 3) // Daca elementul nu este root ii punem idendare de atatea ori de cat este nevoie
//	{
//		outFile << "  ";
//		depth--;
//	}
//	outFile << node->getTag().toString().c_str() << "\t";  //Tag
//	DcmVR vrrr = node->getVR(); // VR
//	outFile << vrrr.getVRName() << "\t"; // VR
//	outFile << std::to_string(node->getLength()).c_str() << "\t"; // Length 
//	outFile << std::to_string(node->getVM()).c_str() << "\t"; // Vm
//	DcmTag tagname = node->getTag(); //  Name
//	outFile << tagname.getTagName() << "\n"; //  Name
//	outFile.close();
//}

//void DCMExtractor::formatFile() const
//// Folosita pentru a sterge din fisierul scos de dcmtk toate liniile care nu contin taguri 
//{
//	ifstream outfile("OriginalFile.txt");
//	ofstream temp("temp.txt"); // Folosim un fisier temporar ce va fi sters ulterior
//	string line;
//	
//	while (getline(outfile, line)) // Citim linie cu linie din fisierul nostru
//	{
//		for (int i = 0; i < line.length(); i++)
//		{
//			if (line[i] == '(') // Daca primul caracter intalnit pe linie este '(' scrie linia aia si sare la linia urmatoare, altfel nu ne intereseaza linia
//			{
//				temp << line << endl;
//				break;
//			}
//		}
//	}
//
//	temp.close();
//	outfile.close();
//	remove("OriginalFile.txt"); // stergem fisierul original
//	rename("temp.txt", "OriginalFile.txt"); // redenumim temporarul cu numele originalului
//}

