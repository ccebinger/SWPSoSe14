
   $(function () {
	  
	    $('#container').highcharts({
	        
	        chart: {
	            type: 'column'
	        },

	        plotOptions: {
                column: {
                    colorByPoint: true
                }
            },
            colors: [
                '#A40B0B',
                '#0BA50B'],

	        
	        title: {
	            text: 'Stacked column chart'
	        },
	        xAxis: {
	            categories: ['C++ zu Haskell', 'Haskell zu C++']
	        },
	        yAxis: {
	            min: 0,
	            title: {
	                text: 'Anzahl Tests'
	            },
	            stackLabels: {
	                enabled: false,
	                style: {
	                    fontWeight: 'bold',
	                    color: (Highcharts.theme && Highcharts.theme.textColor) || 'gray'
	                }
	            }
	        },
	        legend: {
	          
	            verticalAlign: 'top',
	            y: 0,
	            floating: true,
	            backgroundColor: (Highcharts.theme && Highcharts.theme.background2) || 'white',
	            borderColor: '#CCC',
	            borderWidth: 1,
	            shadow: false
	        },
	        tooltip: {
	            formatter: function () {
	                return '<b>' + this.x + '</b><br/>' + this.series.name + ': ' + this.y + '<br/>' +
	                    'Total: ' + this.point.stackTotal;
	            }
	        },
	        plotOptions: {
	            column: {
	                stacking: 'normal',
	                dataLabels: {
	                    enabled: true,
	                    color: (Highcharts.theme && Highcharts.theme.dataLabelsColor) || 'white',
	                    style: {
	                        textShadow: '0 0 3px black, 0 0 3px black'
	                    }
	                }
	            }
	        },
	        series: [{
	            name: 'Fehlgeschlagene Test',
	            data: [<?php echo( $CppToHaColumn->Failed.",".$HaToCppColumn->Failed )?>]
	        }, {
	            name: 'Erfolgreiche Tests ',
	            data: [<?php echo( $CppToHaColumn->Valid.",".$HaToCppColumn->Valid) ?>]
	        }]
	    });
	});


   $(function () {
		  
	    $('#container').highcharts({
	        
	        chart: {
	            type: 'column'
	        },

	        plotOptions: {
               column: {
                   colorByPoint: true
               }
           },
           colors: [
               '#A40B0B',
               '#0BA50B'],

	        
	        title: {
	            text: 'Stacked column chart'
	        },
	        xAxis: {
	            categories: ['C++ zu Haskell', 'Haskell zu C++']
	        },
	        yAxis: {
	            min: 0,
	            title: {
	                text: 'Anzahl Tests'
	            },
	            stackLabels: {
	                enabled: false,
	                style: {
	                    fontWeight: 'bold',
	                    color: (Highcharts.theme && Highcharts.theme.textColor) || 'gray'
	                }
	            }
	        },
	        legend: {
	          
	            verticalAlign: 'top',
	            y: 0,
	            floating: true,
	            backgroundColor: (Highcharts.theme && Highcharts.theme.background2) || 'white',
	            borderColor: '#CCC',
	            borderWidth: 1,
	            shadow: false
	        },
	        tooltip: {
	            formatter: function () {
	                return '<b>' + this.x + '</b><br/>' + this.series.name + ': ' + this.y + '<br/>' +
	                    'Total: ' + this.point.stackTotal;
	            }
	        },
	        plotOptions: {
	            column: {
	                stacking: 'normal',
	                dataLabels: {
	                    enabled: true,
	                    color: (Highcharts.theme && Highcharts.theme.dataLabelsColor) || 'white',
	                    style: {
	                        textShadow: '0 0 3px black, 0 0 3px black'
	                    }
	                }
	            }
	        },
	        series: [{
	            name: 'Fehlgeschlagene Test',
	            data: [<?php echo( $CppToHaColumn->Failed.",".$HaToCppColumn->Failed )?>]
	        }, {
	            name: 'Erfolgreiche Tests ',
	            data: [<?php echo( $CppToHaColumn->Valid.",".$HaToCppColumn->Valid) ?>]
	        }]
	    });
	});
   $(function () {
	    var chart;
	    
	    $(document).ready(function () {
	    	
	    	// Build the chart
	        $('#container2').highcharts({
	            chart: {
	                plotBackgroundColor: null,
	                plotBorderWidth: null,
	                plotShadow: false
	            },
	            title: {
	                text: ''
	            },
	            tooltip: {
	        	    pointFormat: '{series.name}: <b>{point.percentage:.1f}%</b>'
	            },
	            plotOptions: {
	                pie: {
	                    allowPointSelect: true,
	                    cursor: 'pointer',
	                    dataLabels: {
	                        enabled: false
	                    },
	                    showInLegend: true
	                }
	            },
	            series: [{
	                type: 'pie',
	                name: 'C++',
	                data: [
	                    ['Failed',   45.0],
	                    
	                    {
	                        name: 'Valid',
	                        y: 12.8,
	                        sliced: true,
	                        selected: true
	                    }
	                ]
	            }]
	        });
	    });
	    
	});