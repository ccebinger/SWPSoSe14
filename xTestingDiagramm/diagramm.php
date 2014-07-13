<?php


class diagramm
{
	public $Valid=0;
	public $Failed=0;

	public function __construct($sql,$type,$idRun)
	{
		$qResultValid= $sql->query("SELECT count(*) as countValid  FROM result where idRun="
				.$idRun
				." and type=".$type." and blame = 0 ;");

		$qResultFailed = $sql->query("SELECT count(*) as countFailed FROM result where idRun="
				.$idRun
				." and type=".$type."  and blame > 0 ;");
		
		$ResultValid=$sql->result(0,'countValid',$qResultValid);
		$ResultFailed=$sql->result(0,'countFailed',$qResultFailed);
		
		$this->Valid  = round(($ResultValid/( $ResultValid + $ResultFailed))*100,2);
		$this->Failed   = round(($ResultFailed/( $ResultValid + $ResultFailed))*100,2);
	}
}



