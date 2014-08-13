package sqlite3_test;

import java.sql.*;

class SQLiteJDBC
{
	public static void connect( String args[] )
	{
		Connection connection = null;
		// load the sqlite-JDBC driver using the current class loader
		try
		{
			Class.forName("org.sqlite.JDBC");    
			// create a database connection
			connection = DriverManager.getConnection("jdbc:sqlite:test.db");
			Statement statement = connection.createStatement();
			statement.setQueryTimeout(30);  // set timeout to 30 sec.

			statement.executeUpdate("drop table if exists person");
			statement.executeUpdate("create table person (id integer, name string)");
			statement.executeUpdate("insert into person values(1, 'leo')");
			statement.executeUpdate("insert into person values(2, 'yui')");
			ResultSet rs = statement.executeQuery("select * from person");
			
			while(rs.next())
			{
				// read the result set
				System.out.println("name = " + rs.getString("name"));
				System.out.println("id = " + rs.getInt("id"));
			}
			
			// insert by prepared statement
			PreparedStatement preStmt = connection.prepareStatement("insert into person values(?,?)");
			preStmt.setInt(0, 3);
			preStmt.setString(1, "zhang");
			preStmt.execute();
		}
		catch(SQLException e)
		{
			// if the error message is "out of memory", 
			// it probably means no database file is found
			System.err.println(e.getMessage());
		}
		catch (Exception e)
		{
			System.err.println(e.getMessage());
		}
		finally
		{
			try
			{
				if(connection != null)
					connection.close();
			}
			catch(SQLException e)
			{
				// connection close failed.
				System.err.println(e);
			}
		}
	}
}


public class sqlite_test 
{
	public static void main(String[] args)       
	{
		// TODO Auto-generated method stub
		SQLiteJDBC.connect(args);
	}
}
