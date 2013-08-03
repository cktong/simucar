info = {
    "team":   "SimuCar",
    "members":   "Moises Coronado, Christopher Tong, Suyang Wang, Derrick Yen, and Mengyi Zhang",
    "description":  
        """This is a simulation demo of the Car Rental Simulation. 
           I hope you enjoy!""",
    "additional":
        """<h3>Additional Info:</h3>
        <ul>
            <li>LOREM IPSUM DOLOR</li>
            <li>LOREM IPSUM DOLOR</li>
            <li>LOREM IPSUM DOLOR</li>
            <li>LOREM IPSUM DOLOR</li>
            <li>LOREM IPSUM DOLOR</li>
        </ul>
        """
}

#Trace Vars:         L[1],L[2],L[3],L[4],L1[1],L1[2],L1[3],L1[4],L2[1],L2[2],L2[3],L2[4],SERVICED[1],SERVICED[2],SERVICED[3],SERVICED[4],RUNC[1],RUNC[2],RUNC[3],RUNC[4],CUMCOST[1],CUMCOST[2],CUMCOST[3],CUMCOST[4]
#,REN[1;0],REN[2;0],REN[3;0],REN[4;0],REN[1;1],REN[2;1],REN[3;1],REN[4;1],TF,FIXT[0],FIXT[1],BREAKC,RUNC[1],RUNC[2],RUNC[3],RUNC[4],PRICEINS[1],PRICEINS[2],PRICEINS[3],PRICEINS[4]

sigma = {
    "model": "carrent0347",    
    "runtime":{"name": "RUNTIME",
               "display": "In days",
               "default": 800
              },
    "parameters": [
        {   "name": "S[1]",
            "display": "Small",
            "default": 7
        },

        {   "name": "S[2]",
            "display": "Medium",
            "default": 10
        },
        
        {   "name": "S[3]",
            "display": "Large",
            "default": 5
        },

        {   "name": "S[4]",
            "display": "Convertible",
            "default": 3
        },
        
        {   "name": "R",
            "display": "Decimal",
            "default": .5
        },

        {   "name": "ARRMM[1;0]",
            "display": "Small",
            "default": 1
        },
        
        {   "name": "ARRMM[2;0]",
            "display": "Medium",
            "default": 2
        },

        {   "name": "ARRMM[3;0]",
            "display": "Large",
            "default": 1
        },
        
        {   "name": "ARRMM[4;0]",
            "display": "Convertible",
            "default": 1
        },

        {   "name": "ARRMM[1;1]",
            "display": "Small",
            "default": 3
        },
        
        {   "name": "ARRMM[2;1]",
            "display": "Medium",
            "default": 5
        },

        {   "name": "ARRMM[3;1]",
            "display": "Large",
            "default": 5
        },
        
        {   "name": "ARRMM[4;1]",
            "display": "Convertible",
            "default": 3
        },

        {   "name": "RES",
            "display": "In days",
            "default": 3
        },
        

        {   "name": "REN[1;0]",
            "display": "Small",
            "default": 1
        },
        
        {   "name": "REN[2;0]",
            "display": "Medium",
            "default": 1
        },

        {   "name": "REN[3;0]",
            "display": "Large",
            "default": 1
        },
        
        {   "name": "REN[4;0]",
            "display": "Convertible",
            "default": 1
        },

        {   "name": "REN[1;1]",
            "display": "Small",
            "default": 10
        },
        
        {   "name": "REN[2;1]",
            "display": "Medium",
            "default": 10
        },

        {   "name": "REN[3;1]",
            "display": "Large",
            "default": 10
        },
        
        {   "name": "REN[4;1]",
            "display": "Convertible",
            "default": 10
        },
        
        {   "name": "TF",
            "display": "Average days",
            "default": 30
        },
        
        {   "name": "FIXT[0]",
            "display": "Min days",
            "default": 1
        },
        
        {   "name": "FIXT[1]",
            "display": "Max days",
            "default": 5
        },
        
        {   "name": "BREAKC",
            "display": "Dollar per day",
            "default": 50
        },
        
        {   "name": "RUNC[1]",
            "display": "Small",
            "default": 70
        },

        {   "name": "RUNC[2]",
            "display": "Medium",
            "default": 90
        },
        
        {   "name": "RUNC[3]",
            "display": "Large",
            "default": 120
        },

        {   "name": "RUNC[4]",
            "display": "Convertible",
            "default": 150
        },
        
        {   "name": "PRICEINS[1]",
            "display": "Small",
            "default": 8000
        },

        {   "name": "PRICEINS[2]",
            "display": "Medium",
            "default": 10000
        },
        
        {   "name": "PRICEINS[3]",
            "display": "Large",
            "default": 15000
        },

        {   "name": "PRICEINS[4]",
            "display": "Convertible",
            "default": 20000
        },  
        
       
    ]
}

rdp_enable = True

graphs = [
    {   "graph_name": "Profit Over Time",
        "x-axis": "Time",
        "x-display": "Time (days)",
        "y-axis": ["NETP[1]","NETP[2]","NETP[3]","NETP[4]"],
        "y-display": "Net Profit",
        "lines": ["Net Profit Type 1", "Net Profit Type 2","Net Profit Type 3","Net Profit Type 4"],
        "rdp_epsilon": [1, 1.5,2,2.5]
    },

    {   "graph_name": "Cumulative Cost Over Time",
        "x-axis": "Time",
        "x-display": "Time (days)",
        "y-axis": ["CUMCOST[1]","CUMCOST[2]","CUMCOST[3]","CUMCOST[4]"],
        "y-display": "Cumulative Cost",
        "lines": ["Cumulative Cost Type 1", "Cumulative Cost Type 2","Cumulative Cost Type 3","Cumulative Cost Type 4"],
        "rdp_epsilon": [1, 1.5,2,2.5]
    },
]

server = {
    "hostname": "localhost",
    "port": 8000
}
