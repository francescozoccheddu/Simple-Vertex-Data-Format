#pragma once

#include "Map.hpp"
#include "Grammar.hpp"
#include "Declaration.hpp"

#include <string>
#include <fstream>
#include <istream>
#include <sstream>
#include <stdexcept>


namespace SVDF
{

	class Parser
	{

	public:

		class Error : public std::runtime_error
		{

		public:

			static constexpr int unknown_column{ -1 };

			const int line;
			const int column;
			Error (const std::string & message, int line, int column);

			const std::string & where_and_what () const;

		private:

			const std::string message;

			std::string make_message () const;

		};

		Parser (std::istream & stream);

		Map next_map ();

		template<typename T, typename = typename enable_if_data_value_t<T> >
		T next_value ();

		template<typename T, typename = typename enable_if_data_value_t<T> >
		DataDeclaration<T> next_declaration ();

		bool has_data () const;

		bool is_compromised () const;

		bool has_declarations ();

		virtual ~Parser () = default;

	private:

		struct State
		{
			int current_line;
			std::streampos last_newline;
			bool compromised;
			bool in_data;
		};

		struct BackupState
		{
			State state;
			std::streampos pos;
		};

		std::istream & stream;
		State state;

		Map consume_map ();

		Key consume_key ();

		void consume (char c);

		char consume ();

		void consume_comment ();

		char peek ();

		bool try_peek (char & out);

		BackupState make_backup () const;

		void restore (const BackupState & backup);

		Error make_error (const std::string message);

	};

	template<typename T, typename _EI>
	inline T Parser::next_value ()
	{
		const BackupState backup{ make_backup () };
		try
		{
			consume_comment ();
			T val;
			stream >> val;
			if (!stream.fail ())
			{
				consume_comment ();
				char c = consume ();
				switch (c)
				{
					case Grammar::value_separator:
						break;
					case Grammar::declaration_suffix:
						state.in_data = false;
						break;
					default:
					{
						std::stringstream ss;
						ss << "Expected value separator '";
						ss << Grammar::value_separator;
						ss << "' or declaration suffix '";
						ss << Grammar::declaration_suffix;
						ss << "'";
						throw make_error (ss.str());
					}
				}
				return val;
			}
			else
			{
				std::stringstream ss;
				ss << "Expected ";
				ss << typeid(T).name ();
				ss << " value";
				throw make_error (ss.str ());
			}
		}
		catch (std::logic_error &)
		{
			restore (backup);
			throw;
		}
	}

	template<typename T, typename _EI>
	inline DataDeclaration<T> Parser::next_declaration ()
	{
		DataDeclaration<T> d{ next_map () };
		d.parse_data (*this);
		return d;
	}

	class FileParser : public Parser
	{

	public:

		FileParser ();

		FileParser (const std::string & _filename);

		void open (const std::string & _filename);

		bool is_open () const;

		void close ();

	private:

		std::ifstream file;

	};

	class StringParser : public Parser
	{

	public:

		StringParser (const std::string & string);

	private:

		std::istringstream stringstream;

	};



}