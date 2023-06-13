#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include "nin-io/util/util.h"
#include "StringList.h"
#include "ExecutionCommand.h"

class CommandBody
{
public:
	CommandBody();
	CommandBody(StringList* string_list);
	~CommandBody();

	void setStringList(StringList* string_list) { m_string_list = string_list; }
	void load(std::fstream& file);
	int getAddress() { return m_address; };

	void setCommandReference(ExecutionCommand* node_command) { m_command = node_command; }
	ExecutionCommand* getCommandReference() { return m_command; }

	struct CallTableEntry {
		int index = -1; // index of the command to call
		std::string parameter = "";
	};

	std::vector<CallTableEntry>* getCallTable() { return &m_call_table; }

	struct Parameter {
		int section_num = -1;
		int index = -1;
		int value = -1;
	};

	std::vector<Parameter>* getTableParameters();
	std::vector<Parameter>* getStructureParameters();

private:
	int m_address;

	StringList* m_string_list = nullptr;

	ExecutionCommand *m_command = nullptr;

	//std::map<int, int> m_table_parameters;
	//std::map<int, int> m_table_values;
	//std::map<int, int> m_structure_parameters;
	//std::map<int, int> m_structure_values;

	std::vector<Parameter> m_table_parameters;
	std::vector<Parameter> m_section_parameters;
	std::vector<CallTableEntry> m_call_table;

	static const bool is_big_endian = false;
};