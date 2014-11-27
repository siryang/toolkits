
namespace Trace
{
	class TraceStream
	{
	public:
		TraceStream()
		{
			m_logFS.open("redc_trace.log", ios_base::app);
		}

		~TraceStream()
		{
			m_logFS.close();
		}

		template<class T>
		TraceStream& operator << (T data)
		{
			cout << data;
			m_logFS << data;
			return *this;
		}

		typedef std::basic_ostream<char, std::char_traits<char>> CoutType;
		typedef CoutType& (*StandardEndLine)(CoutType&);
		TraceStream& operator<< (StandardEndLine sln)
		{
			sln(std::cout);
			sln(m_logFS);
			return *this;
		}

	private:
		ofstream m_logFS;
	};
	extern TraceStream redcLog;
}

using namespace Trace;


//// move to .cpp
//namespace Trace
//{
//	TraceStream redcLog;
//}