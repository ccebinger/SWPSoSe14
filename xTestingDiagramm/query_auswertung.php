<?php

class qAuswetung
{


	public function __construct()
	{
	}
	
	public function anzahl_test($sql,$type = 0, $idRun = 0){

		if($idRun === 0){
		$res_t_cc = $sql->query("SELECT * FROM result where type = ".$type .";");
		
		}
		else {
			$res_t_cc = $sql->query("SELECT * FROM result where type = ".$type ." and idRun=".$idRun.";");
		}



		return($sql->num_result($res_t_cc));
		$sql->free_result();

	}

	public function query($sql,$sqlcode)
	{
		$sql->last_injection = mysql_query($sqlcode);

		if($sql->last_injection === false)
		{
			$message  = "Fehler bei dem Ausf&uuml;hren eines Mysql-codes!<br />\n";
			$message .= "Mysql-Code: " . htmlspecialchars($sqlcode, ENT_QUOTES) . "<br />\n";
			$message .= "Mysql-fehlermeldung:<br />\n";
			$message .= mysql_error();
			trigger_error($message);
		}

		return($this->last_injection);
	}

	

}



?>