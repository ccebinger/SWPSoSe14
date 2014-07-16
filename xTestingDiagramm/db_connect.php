<?php
//Connect To Database


$hostname='';
$username='';
$password='';
$dbname='';





class mysql
{
	private $host     = '';
	private $user     = '';
	private $passwort     = '';
	private $dbname     = '';
	private $last_injection = '';
	private $conn_id = null;




	public function __construct($host, $user, $passwort, $dbname)
	{
		$this->host     = $host;
		$this->user     = $user;
		$this->passwort = $passwort;
		$this->dbname     = $dbname;
		$this->connect_mysql();
		return($this->conn_id);
	}



	private function connect_mysql()
	{
		$this->conn_id = mysql_connect($this->host,$this->user,$this->passwort);

		if($this->conn_id === false)
		{
			$message  = "Verbindung zur Datenbank nicht m&ouml;glich.<br />\n";
			$message .= "Mysql-fehlermeldung: <br />\n";
			$message .= mysql_error();

			trigger_error($message);
		}
		else
		{
			$this->select_db();
		}
	}



	private function select_db()
	{
		$select = mysql_select_db($this->dbname,$this->conn_id);

		if($select === false)
		{
			$message  = "Die angegebene Datenbank \"".$this->dbname."\" existiert nicht.<br />\n";
			$message .= "Mysql-fehlermeldung: <br />\n";
			$message .= mysql_error();

			trigger_error($message);
		}
	}



	public function query($sqlcode)
	{
		$this->last_injection = mysql_query($sqlcode);

		if($this->last_injection === false)
		{
			$message  = "Fehler bei dem Ausf&uuml;hren eines Mysql-codes!<br />\n";
			$message .= "Mysql-Code: " . htmlspecialchars($sqlcode, ENT_QUOTES) . "<br />\n";
			$message .= "Mysql-fehlermeldung:<br />\n";
			$message .= mysql_error();
			trigger_error($message);
		}

		return($this->last_injection);
	}



	public function array_result($sql = NULL, &$row = '')
	{
		$inc = '';
		if($sql === NULL)
		{
			$inc = $this->last_injection;
		} else {
			$inc = $sql;
		}

		$row = mysql_fetch_array($inc);

		return($row);
	}



	public function row_result($sql = NULL, &$row = '')
	{
		$inc = '';
		if($sql === NULL)
		{
			$inc = $this->last_injection;
		} else {
			$inc = $sql;
		}

		$row = mysql_fetch_row($inc);

		return($row);
	}



	public function object_result($sql = NULL, &$row = '')
	{
		$inc = '';
		if($sql === NULL)
		{
			$inc = $this->last_injection;
		} else {
			$inc = $sql;
		}

		$row = mysql_fetch_object($inc);

		return($row);
	}



	public function assoc_result($sql = NULL, &$row = '')
	{
		$inc = '';
		if($sql === NULL)
		{
			$inc = $this->last_injection;
		} else {
			$inc = $sql;
		}

		$row = mysql_fetch_assoc($inc);

		return($row);
	}



	public function num_result($sql = NULL)
	{
		$inc = '';
		if($sql === NULL)
		{
			$inc = $this->last_injection;
		} else {
			$inc = $sql;
		}

		$num = mysql_num_rows($inc);

		return($num);
	}



	public function sql_string($string)
	{
		return(mysql_real_escape_string($string));
	}



	public function free_result($sql = NULL)
	{
		$inc = '';
		if($sql === NULL)
		{
			$inc = $this->last_injection;
		} else {
			$inc = $sql;
		}

		mysql_free_result($inc);
	}



	public function result($set = 0, $field = 0, $sql = NULL, &$row = '')
	{
		$inc = '';
		if($sql === NULL)
		{
			$inc = $this->last_injection;
		} else {
			$inc = $sql;
		}

		$row = mysql_result($inc, $set, $field);

		return($row);
	}



	public function insert_id(&$row = '')
	{

		$row = mysql_insert_id();

		return($row);
	}



	public function close_connect()
	{
		mysql_close($this->conn_id);
	}
}


?>