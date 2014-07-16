<?php


class diagramm
{
	public $Valid=0;
	public $Failed=0;

	public function __construct($sql, $type, $idRun)
	{
		$qResultValid= $sql->query("SELECT COUNT(*) as countValid FROM result WHERE idRun='".$idRun."' AND type=".$type." AND blame = 0 ;");

		$qResultFailed = $sql->query("SELECT COUNT(*) as countFailed FROM result WHERE idRun='".$idRun."' AND type=".$type." AND blame > 0 ;");
		
		$ResultValid=$sql->result(0,'countValid',$qResultValid);
		$ResultFailed=$sql->result(0,'countFailed',$qResultFailed);
		
		$this->Valid  = round(($ResultValid/( $ResultValid + $ResultFailed))*100);
		$this->Failed   = round(($ResultFailed/( $ResultValid + $ResultFailed))*100);
	}
}



