#include<fstream>
#include<cstring>
#include<sstream>
#include<iostream>
#include<iomanip>
#include<stdlib.h>
using namespace std;

fstream source, list, file_loc, loc, symtab, obj;
string opcode_found(string op);
void location_table(string location_file, string file_name_lst);
void symbol_table(string location_file, string file_name_stb);
void object_code(string file_name_asm, string file_name_obj);
string dec_to_hex(string num);

int main() {
	string file_name_and_mode;
	string file_name, mode, file_name_asm, file_name_lst, file_name_stb, file_name_obj, file_location;

	cout << "=================================================================" << endl;
	cout << "|                      ONE PASS SIC ASSEMBLER                   |" << endl;
	cout << "=================================================================" << endl;
	cout << "|   Please enter the file name and mode                         |" << endl;
	cout << "|   1.File_Name       | Output File_Name.obj                    |" << endl;
	cout << "|   2.File_Name -s    | Output File_Name.obj and File_Name.lst  |" << endl;
	cout << "|   2.File_Name -t    | Output File_Name.obj and File_Name.stb  |" << endl;
	cout << "|   2.File_Name -a    | Output three file                       |" << endl;
	cout << "=================================================================" << endl;
	cout << "=> ";
	getline(cin, file_name_and_mode);
	stringstream FNAM(file_name_and_mode);
	getline(FNAM, file_name, ' ');
	getline(FNAM, mode);

	if (file_name[file_name.size() - 4] != '.') {
		file_name_asm = file_name + ".asm";
		file_name_lst = file_name + ".lst";
		file_name_stb = file_name + ".stb";
		file_name_obj = file_name + ".obj";
	}
	else {
		file_name_asm = file_name;

		file_name_lst = file_name;
		file_name_lst[file_name_lst.size() - 2] = 'l';
		file_name_lst[file_name_lst.size() - 1] = 's';
		file_name_lst[file_name_lst.size()] = 't';

		file_name_stb = file_name;
		file_name_stb[file_name_stb.size() - 2] = 's';
		file_name_stb[file_name_stb.size() - 1] = 't';
		file_name_stb[file_name_stb.size()] = 'b';

		file_name_obj = file_name;
		file_name_obj[file_name_obj.size() - 2] = 'o';
		file_name_obj[file_name_obj.size() - 1] = 'b';
		file_name_obj[file_name_obj.size()] = 'j';
	}

	if (mode == "") {
		object_code(file_name_asm, file_name_obj);
	}
	else if (mode == "-s") {
		object_code(file_name_asm, file_name_obj);
		location_table("file_location.txt", file_name_lst);
	}
	else if (mode == "-t") {
		object_code(file_name_asm, file_name_obj);
		symbol_table("file_location.txt",file_name_stb);
	}
	else if (mode == "-a") {
		object_code(file_name_asm, file_name_obj);
		location_table("file_location.txt", file_name_lst);
		symbol_table("file_location.txt",file_name_stb);
	}
	else {
		cout << "Error Enter!!!" << endl;
		exit(1);
	}

	system("pause");
	return 0;
}

string opcode_found(string op)
{
	string value;
	string code;
	string opcode_table[] = {
"ADD	18","ADDF	58","ADDR	90","AND	40","CLEAR	B4","COMP	28"
,"COMPF	88","COMPR	A0","DIV	24","DIVF	64","DIVR	9C","FIX	C4","FLOAT	C0","HIO	F4","J	3C","JEQ	30"
,"JGT	34","JLT	38","JSUB	48","LDA	00","LDB	68","LDCH	50","LDF	70","LDL	08","LDS	6C","LDT	74"
, "LDX	04","LPS	D0","MUL	20","MULF	60","MULR	98","NORM	C8","OR	44","RD	D8","RMO	AC","RSUB	4C","SHIFTL	A4"
,"SHIFTR	A8","SIO	F0","SSK	EC","STA	0C","STB	78","STCH	54","STF	80","STI	D4","STL	14","STS	7C"
,"STSW	E8","STT	84","STX	10","SUB	1C","SUBF	5C","SUBR	94","SVC	B0","TD	E0","TIO	F8","TIX	2C",
"TIXR	B8","WD	DC"
	};

	for (int i = 0; i < 59; i++) {
		stringstream iss(opcode_table[i]);
		getline(iss, code, '\t');
		getline(iss, value);
		if (op == code) 
			return value;
	}
	return "null";
}

void location_table(string location_file, string file_name_lst)
{
	loc.open(file_name_lst, ios::out);
	file_loc.open(location_file, ios::in);
	string line, location, label, opcode, operand;
	loc << "Loc.    Source       statement" << endl
		<< "=====   ==================================" << endl;
	while (getline(file_loc, line)) {
		stringstream str(line);
		getline(str, location, '\t');
		getline(str, label, '\t');
		getline(str, opcode, '\t');
		getline(str, operand, '\t');
		loc << setw(5) << setfill('0') << location << setw(2) << setfill(' ');
		loc << setw(8) << label << setw(7) << opcode << setw(15) << operand << endl;
	}
		
	loc.close();
	file_loc.close();
}
void symbol_table(string location_file, string file_name_stb) {
	symtab.open(file_name_stb, ios::out);
	file_loc.open(location_file, ios::in);
	string line, loc, label, opcode, operand;
	symtab << "Symbal   Value" << endl
		<< "======   ============" << endl;
	while (getline(file_loc, line)) {
		stringstream str(line);
		getline(str, loc, '\t');
		getline(str, label, '\t');
		getline(str, opcode, '\t');
		getline(str, operand, '\t');
		if (label != "" and label != "START" and label[0] != '.')
			symtab << setw(10) << setfill(' ') << left << label << "\t" << setw(5) << setfill('0') << right << loc << endl;
	}
	symtab.close();
	file_loc.close();
}
void object_code(string file_name_asm, string file_name_obj) 
{
	source.open(file_name_asm, ios::in);
	if (!source) {
		cout << "Not open the file!!!" << endl;
		exit(1);
	}
	loc.open("file_location.txt", ios::out);
	obj.open(file_name_obj, ios::out);
	string symtable[70];
	string temp, location, line, label, opcode, operand, current;
	int start_address = 0, location_count = 0, count = 0, index = 0, sym_count = 0;
	string address_loc[100], address_label[100];
	string not_define[100], not_address[100];		//The operand that not define yet
	string refine;
	int define_count = 0;
	int size_count = 0;

	getline(source, line);
	stringstream cut_temp(line);
	getline(cut_temp, label, '\t');
	getline(cut_temp, opcode, '\t');
	getline(cut_temp, operand, '\t');
	if (opcode == "START") {
		stringstream str;
		str << setw(6) << setfill('0') << hex << operand;
		str >> start_address;
		location_count = start_address;
		loc << setw(4) << setfill('0') << hex << location_count << "\t";
		loc << label << "\t" << opcode << "\t" << operand << endl;
		obj << "H" << label << " " << setw(6) << setfill('0') << hex << start_address;
	}
	label.clear();
	opcode.clear();
	operand.clear();

	while (getline(source, line)) {
		string value;
		stringstream cut_temp(line);
		getline(cut_temp, label, '\t');
		getline(cut_temp, opcode, '\t');
		getline(cut_temp, operand, '\t');
		if (label[0] == '.')
			loc << "\t" << label << " " << opcode << endl;
		else
		{
			value = opcode_found(opcode);
			if (value != "null") {
				loc << setw(4) << setfill('0') << hex << location_count << "\t";
				loc << label << "\t" << opcode << "\t" << operand << endl;
				location_count += 3;
			}
			else if (opcode == "WORD") {
				loc << setw(4) << setfill('0') << hex << location_count << "\t";
				loc << label << "\t" << opcode << "\t" << operand << endl;
				location_count += 3;
			}
			else if (opcode == "RESW") {
				loc << setw(4) << setfill('0') << hex << location_count << "\t";
				loc << label << "\t" << opcode << "\t" << operand << endl;
				stringstream num(operand);
				int number = 0;
				num >> number;
				location_count += 3 * number;
			}
			else if (opcode == "BYTE") {
				loc << setw(4) << setfill('0') << hex << location_count << "\t";
				loc << label << "\t" << opcode << "\t" << operand << endl;
				string name, ch;
				int number = 0;
				stringstream temp(operand);
				getline(temp, name, '\'');
				getline(temp, ch, '\'');
				if (name == "C" || name == "c")
					number = ch.size();
				else if (name == "X" || name == "x")
					number = 1;
				location_count += number;
			}
			else if (opcode == "RESB") {
				loc << setw(4) << setfill('0') << hex << location_count << "\t";
				loc << label << "\t" << opcode << "\t" << operand << endl;
				stringstream num(operand);
				int number;
				num >> number;
				location_count += number;
			}
			else if (opcode == "END") {
				loc << setw(4) << setfill('0') << hex << location_count << "\t";
				loc << label << "\t" << opcode << "\t" << operand << endl;
				obj << setw(4) << setfill('0') << hex << location_count - start_address << endl;
			}
		}
		label.clear();
		opcode.clear();
		operand.clear();
	}
	//location
	loc.close();

	list.open("file_location.txt", ios::in);
	if (!list) {
		cout << "Not open the file !!!" << endl;
		exit(1);
	}


	getline(list, temp);
	stringstream cut(temp);
	getline(cut, location, '\t');
	getline(cut, label, '\t');
	getline(cut, opcode, '\t');
	getline(cut, operand, '\t');
	string first_loc = location;


	if (opcode == "START") {
		loc << setw(5) << setfill('0') << hex << location << "\t";
		loc << label << "\t" << opcode << "\t" << operand << endl;
		location.clear();
		label.clear();
		opcode.clear();
		operand.clear();
	}

	while (getline(list, temp)) {
		string value;
		stringstream cut(temp);
		getline(cut, location, '\t');
		getline(cut, label, '\t');
		getline(cut, opcode, '\t');
		getline(cut, operand, '\t');	//Seperate location,label,opcode and operand

		if (label != ""&&label[0] != '.') {
			symtable[sym_count].append(label);
			symtable[sym_count].append("\t");
			symtable[sym_count].append(location);
			sym_count++;
		}
		value = opcode_found(opcode);
		if (opcode != "END") {
			for (int i = 0; i < define_count; i++) {	//If the current label is s
				if (label == not_define[i]) {
					if (current != "") {
						obj << setw(2) << hex << size_count / 2 << current << endl;
						current.clear();
						count = 0;
						size_count = 0;
					}
					stringstream ss;
					int address_temp;
					ss << not_address[i];
					ss >> hex >> address_temp;
					stringstream sss;
					sss << hex << address_temp + 1;
					string num(sss.str());
					obj << 'T' << setw(6) << setfill('0') << num << "02" << setw(4) << location << endl;
					not_define[i] = "/";
					not_address[i] = "/";
				}
			}
			if (count == 10 || label == "FIRST") {
				obj << setw(2) << hex << size_count / 2 << current << endl;
				current.clear();
				count = 0;
				size_count = 0;
			}
			if (count == 0) {
				obj << 'T' << setw(6) << hex << location;
			}

			if (opcode == "BYTE") {		//find the opcode
				if (label == "EOF") {
					current.append("454F46");
					size_count += 6;
					count++;
				}

				else {
					current.append(operand.substr(2, 2));
					count++;
					size_count += 2;
				}
			}
			else if (opcode == "WORD") {
				string k;
				for (int i = 0; i < 6 - operand.size(); i++)k.append("0");	//Make "00000x" form for WORD opcode
				k.append(dec_to_hex(operand));
				current.append(k);
				size_count += 6;
				count++;
			}
			else if (opcode == "RSUB") {
				current.append(value);
				current.append("0000");
				size_count += 6;
			}
			else if (opcode != "RESW" && opcode != "RESB" && opcode != "") {		//Other opcode form
				string operand_form, x, sym_label, sym_loc;
				int flag = 0;
				stringstream operand_check(operand);
				getline(operand_check, operand_form, ',');
				getline(operand_check, x);
				current.append(value);

				for (int i = 0; i < sym_count; i++) {
					stringstream sym(symtable[i]);
					getline(sym, sym_label, '\t');	//label and location in symbol table
					getline(sym, sym_loc);
					if (operand_form == sym_label) {
						flag = 1;
						break;
					}
				}
				if (flag == 1) {
					if (x == "X") {
						stringstream ss;
						int address_temp = 0;
						ss << sym_loc;
						ss >> hex >> address_temp;
						stringstream sss;
						sss << hex << address_temp + 32768;
						string num(sss.str());
						current.append(num);
					}
					else {
						current.append(sym_loc);
					}
				}
				else {
					current.append("0000");			//No correspond address-> add 0000
					not_define[define_count] = operand_form;
					not_address[define_count] = location;
					define_count++;
				}
				count++;
				size_count += 6;
			}
		}
		else {	//END
			obj << setw(2) << hex << size_count / 2 << current << endl;
			obj << 'E' << setw(6) << setfill('0') << hex << first_loc << endl;		//End record
			break;
		}
		label.clear();
		opcode.clear();
		operand.clear();
	}
	list.close();
	loc.close();
}
string dec_to_hex(string num) {
	stringstream ss(num);
	int a;
	ss >> a;
	stringstream sss;
	sss << hex << a;
	return (sss.str());
}