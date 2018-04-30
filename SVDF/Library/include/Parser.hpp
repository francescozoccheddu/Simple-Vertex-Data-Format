#pragma once

#include "Map.hpp"
#include "Grammar.hpp"
#include "Declaration.hpp"
#include "Utils.hpp"

#include <typeinfo>
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

		static constexpr int infinite{ -1 };

		Parser (std::istream & stream);

		Map next_map ();

		template<typename T, typename = typename SVDF::enable_if_data_value_t<T> >
		T next_value ();

		template<typename T, typename = typename SVDF::enable_if_data_value_t<T> >
		int next_data (T * data, int max);

		template<typename T, typename = typename SVDF::enable_if_data_value_t<T> >
		int next_data (std::vector<T> & data, int max = infinite);

		template<typename T, typename = typename SVDF::enable_if_data_value_t<T> >
		int next_data (DataDeclaration<T> & data, int max = infinite);

		template<typename T, typename = typename SVDF::enable_if_data_value_t<T> >
		DataDeclaration<T> next_declaration ();

		bool has_data () const;

		bool is_compromised () const;

		bool has_declarations ();

		const std::istream & get_stream () const;

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

		template<typename T, typename = typename SVDF::enable_if_data_value_t<T> >
		T consume_value ();

		template<typename T, typename = typename SVDF::enable_if_data_value_t<T> >
		int consume_data (T * data, int max);

		template<typename T, typename = typename SVDF::enable_if_data_value_t<T> >
		int consume_data (std::vector<T> & data, int max);

		Map consume_map ();

		Key consume_key ();

		void consume (char c);

		char consume ();

		bool consume_comment ();

		char peek ();

		bool try_peek (char & out);

		BackupState make_backup () const;

		void restore (const BackupState & backup);

		Error make_error_abs (const std::string & message, std::streampos pos);

		Error make_error_rel (const std::string & message, int col_offset);

	};

	template<typename T, typename>
	inline T Parser::consume_value ()
	{
		consume_comment ();
		std::streampos pos = stream.tellg ();
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
					std::ostringstream ss;
					ss << "Bad character '";
					ss << str_utils::inline_space (c);
					ss << "' for ";
					ss << typeid(T).name ();
					ss << " value. Expected value separator '";
					ss << Grammar::value_separator;
					ss << "' or declaration suffix '";
					ss << Grammar::declaration_suffix;
					ss << "'.";
					throw make_error_rel (ss.str (), -1);
				}
			}
			return val;
		}
		else
		{
			std::ostringstream ss;
			ss << "Value parsing failed. Expected ";
			ss << typeid(T).name ();
			ss << " value.";
			throw make_error_abs (ss.str (), pos);
		}
	}


	template<typename T, typename>
	inline T Parser::next_value ()
	{
		const BackupState backup{ make_backup () };
		try
		{
			return consume_value<T> ();
		}
		catch (Error &)
		{
			restore (backup);
			throw;
		}
	}

	template<typename T, typename>
	inline int Parser::consume_data (T * _data, int _max)
	{
		int count = 0;
		while (has_data () && (_max == infinite || count < _max))
		{
			_data[count] = consume_value<T> ();
			count++;
		}
		return count;
	}

	template<typename T, typename>
	inline int Parser::consume_data (std::vector<T> & _data, int _max)
	{
		int count = 0;
		while (has_data () && (_max == infinite || count < _max))
		{
			_data.push_back (consume_value<T> ());
			count++;
		}
		return count;
	}

	template<typename T, typename>
	inline int Parser::next_data (T * _data, int _max)
	{
		BackupState backup{ make_backup () };
		try
		{
			return consume_data (_data, _max);
		}
		catch (Error &)
		{
			restore (backup);
			throw;
		}
	}

	template<typename T, typename>
	inline int Parser::next_data (std::vector<T>& _data, int _max)
	{
		BackupState backup{ make_backup () };
		try
		{
			return consume_data (_data, _max);
		}
		catch (Error &)
		{
			restore (backup);
			throw;
		}
	}

	template<typename T, typename>
	inline int Parser::next_data (DataDeclaration<T>& _data, int _max)
	{
		return next_data (_data.data, _max);
	}

	template<typename T, typename>
	inline DataDeclaration<T> Parser::next_declaration ()
	{
		BackupState backup{ make_backup () };
		try
		{
			DataDeclaration<T> d{ consume_map () };
			consume_data (d.data, infinite);
			return d;
		}
		catch (Error &)
		{
			restore (backup);
			throw;
		}
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
