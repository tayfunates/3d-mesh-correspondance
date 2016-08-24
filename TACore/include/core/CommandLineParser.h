/**
* @file	CommandLineParser.h
*
* @brief	Header only - minimal command line argument parser support for C++.
*/

#ifndef TACORE_COMMAND_LINE_PARSER_H
#define	TACORE_COMMAND_LINE_PARSER_H

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <sstream>

namespace TACore
{

	class ArgParser
	{
	public:
		class Arg
		{
		public:
			static const int   REMAINING = -1;
			static const int   NOPARAMS = 0;

			std::string		description;
			std::string		name;
			std::string		shortName;
			bool			required;
			bool			exists;
			int				count;
			std::vector<std::string> values;
			std::vector<std::string> defaults;

			Arg(const std::string &name, const std::string &shortName = "", bool required = true, int count = 1, const std::string &description = "")
			{
				this->name = name;
				this->shortName = shortName;
				this->required = required;
				this->count = count;
				this->description = description;
				this->exists = false;
			}

			virtual ~Arg() {}

			virtual std::string argInfo()
			{
				std::string up = this->name;
				std::transform(up.begin(), up.end(), up.begin(), ::toupper);
				std::string info = std::string("--" + this->name);
				if (!this->shortName.empty()) info += std::string(",-" + this->shortName);
				if (this->count > 0)
				{
					for (int i = 0; i < count; ++i) info += " " + up;
				}
				else if (this->count == REMAINING)
				{
					info += " " + up + "...";
				}
				return info;
			}

			virtual bool read(char argc, char **argv, int &idx)
			{
				if (idx + count > argc)
				{
					std::cerr << this->name << " arg requires " << this->count << " parameters." << std::endl;
					return false;
				}
				values.clear();
				this->exists = true;

				if (this->count == NOPARAMS)
				{
					values.push_back("1");
				}
				else
				{
					int lastIdx = count > 0 ? idx + count : argc;
					while (idx < lastIdx)
					{
						values.push_back(argv[idx++]);
					}
				}
				return true;
			}

			virtual std::string getValue(size_t idx = 0)
			{
				std::vector<std::string> &v = this->exists ? this->values : this->defaults;
				return idx < v.size() ? v[idx] : "";
			}

			// Set default values, delimited by "delim"
			virtual void setDefault(const std::string &value, const char delim = '|')
			{
				this->defaults.clear();
				if (value.empty())
					this->defaults.push_back("");
				else
				{
					std::stringstream ss(value);
					std::string item;
					while (std::getline(ss, item, delim)) this->defaults.push_back(item);
					if (*value.rbegin() == delim) this->defaults.push_back("");
				}
			}

			virtual void setDefault(const std::vector<std::string> &values)
			{
				this->defaults.clear();
				this->defaults = values;
			}
		};


	private:
		std::vector<Arg*> args;
		std::string name;
		std::string description;

		bool hasArg(const std::string &name, const std::string &shortName = "")
		{
			for (size_t i = 0; i<this->args.size(); i++)
			{
				Arg *pArg = this->args[i];
				if (!name.empty() && pArg->name == name) return true;
				if (!shortName.empty() && pArg->shortName == shortName) return true;
			}
			return false;
		}

	public:

		ArgParser(const std::string &name, const std::string &description)
		{
			this->name = name;
			this->description = description;
			addArg("help", "h", false, Arg::NOPARAMS, "Display help");
		}

		virtual ~ArgParser()
		{
			for (size_t i = 0; i<this->args.size(); i++)
			{
				delete this->args[i];
			}
		}

		Arg* arg(const std::string &opt, bool hasDashes = false)
		{
			if (hasDashes)
			{
				if (opt.empty() || opt.size() < 2 || (opt.at(0) != '-')) return NULL;
				bool shortName = (opt.at(1) != '-');
				std::string name = shortName ? opt.substr(1) : opt.substr(2);
				for (size_t i = 0; i<this->args.size(); i++)
				{
					Arg *pArg = this->args[i];
					if (shortName && this->args[i]->shortName == name) return pArg;
					if (!shortName && this->args[i]->name == name) return pArg;
				}
			}
			else
			{
				for (size_t i = 0; i<this->args.size(); i++)
				{
					Arg *pArg = this->args[i];
					if (pArg->name == opt) return pArg;
				}
			}
			return NULL;
		}

		bool addArg(const std::string &name, const std::string &shortName = "", bool required = true, int count = 1, const std::string &defaultValue = "", const std::string &description = "")
		{
			if (hasArg(name, shortName)) return false;
			Arg *pArg = new Arg(name, shortName, required, count, description);
			pArg->setDefault(defaultValue);
			this->args.push_back(pArg);
			return true;
		}

		bool addArg(const std::string &name, const std::string &shortName = "", bool required = true, int count = 1, const std::vector<std::string> &defaultValues = std::vector<std::string>(), const std::string &description = "")
		{
			if (hasArg(name, shortName)) return false;
			Arg *pArg = new Arg(name, shortName, required, count, description);
			pArg->setDefault(defaultValues);
			this->args.push_back(pArg);
			return true;
		}

		void printHelp()
		{
			std::cout << this->name << ": " << this->description << std::endl;
			std::cout << this->name << " ";
			for (size_t i = 0; i<this->args.size(); i++)
			{
				std::cout << "[" << this->args[i]->argInfo() << "] ";
			}
			std::cout << "\n\nDescription:\n";
			for (size_t i = 0; i<this->args.size(); i++)
			{
				std::cout << std::setw(30) << std::left << this->args[i]->argInfo() << " " << this->args[i]->description << std::endl;
			}
		}

		bool parseCommandLine(int argc, char **argv)
		{
			int idx = 1;
			// Init
			for (size_t i = 0; i<this->args.size(); i++)
			{
				this->args[i]->exists = false;
			}

			// Parse   
			while (idx < argc)
			{
				std::string arg = argv[idx++];
				Arg* pArg = this->arg(arg, true);
				if (!pArg)
				{
					std::cerr << "Unknown arg " << arg << std::endl;
					return false;
				}
				if (!pArg->read(argc, argv, idx))
					return false;
			}

			// Check help
			if (getBool("help"))
			{
				printHelp();
				exit(0);
			}

			// Check required params
			for (size_t i = 0; i<this->args.size(); i++)
			{
				Arg *pArg = this->args[i];
				if (!pArg->exists && pArg->required)
				{
					std::cerr << pArg->name << " arg is required" << std::endl;
					return false;
				}
			}

			return true;
		}

		int count(const std::string &name)
		{
			Arg *pArg = arg(name, false);
			return pArg ? (int)pArg->values.size() : 0;
		}

		bool exists(const std::string &name)
		{
			Arg *pArg = arg(name, false);
			return pArg ? pArg->exists : false;
		}

		std::string get(const std::string &name, int idx = 0)
		{
			Arg *pArg = arg(name, false);
			return pArg ? pArg->getValue(idx) : "";
		}

		bool getBool(const std::string &name, int idx = 0)
		{
			return !get(name, idx).empty() && get(name, idx) == "1";
		}

		double getDouble(const std::string &name, int idx = 0)
		{
			return get(name, idx).empty() ? 0 : atof(get(name, idx).c_str());
		}

		int getInt(const std::string &name, int idx = 0)
		{
			return get(name, idx).empty() ? 0 : atoi(get(name, idx).c_str());
		}
	};

} // namespace TACore

#endif	
