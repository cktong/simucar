from __future__ import division

from bottle import *
import settings
import util.compile as compile
from app import clean_tmp, run_simulation, prepare_graphs


# Home Page
@get('/')
def index():  
    return template("index", {"info": settings.info})

@get('/login')
def login():
    return template("login",{"info": settings.info})
    
@get('/home')
def home():
   return template("home",{"info": settings.info})

@get('/tos')
def terms():
   return template("tos",{"info": settings.info})
   
# Simulate Page
@get('/simulate')
def simulate_get():  
    return template("simulate", {"info": settings.info, 
                                 "sigma": settings.sigma})
								 
# Beta Distribution Page
@get('/mybeta')
def get_beta():
	return template("mybeta", {"info": settings.info})
                         
@post('/simulate')
def simulate_post():
    parsed, xlsfile_name, last_line = run_simulation(request.forms, settings.sigma)
    out = prepare_graphs(parsed, settings.graphs, settings.rdp_enable)
    
    print last_line
    test= "blah"
    
    #array with all the data of lost servce, serviced etc. Same order as outfile
    data = last_line.split()
    
    print data
    
    lost= []
    serviced=[]
    servicelvls=[]

    # for loop only goes through total lost columns (ie: L4)
    
    for i in range(3,7):
      lost.append(long(data[i]))
      serviced.append(long(data[i+12]))
      servicelvls.append((serviced[i-3])/(serviced[i-3]+lost[i-3]))
      #print lost[i-3], serviced[i-3], servicelvls[i-3]
    print servicelvls
    
    totprofit=0
    for i in range(27,31):
      totprofit=totprofit+float(data[i])   
    
    totalprofit= "%0.2f" % totprofit
    totalprofitstr= '$'+ str(totalprofit)
    
    totservice=(serviced[0]+serviced[1]+serviced[2]+serviced[3])/(lost[0]+lost[1]+lost[2]+lost[3]+serviced[0]+serviced[1]+serviced[2]+serviced[3])
      
    totalservice="%.2f" % totservice
    
    # bar graph for total service
    bar_data = {"key": "Bargraph", 
                "values": [
                    {"value": servicelvls[0], "label": "Car Type 1"},
                    {"value": servicelvls[1], "label": "Car Type 2"},
                    {"value": servicelvls[2], "label": "Car Type 3"},
                    {"value": servicelvls[3], "label": "Car Type 4"},
                   ]}

    out["bar_data"] = bar_data 
    
    
    return template("results", {"info": settings.info,
                                "bind": out,
                                "parameters": request.forms,
                                "settings_params": settings.sigma,
                                "test": test,
                                "xlsfile_name":xlsfile_name,
                                "totprofit": totalprofitstr,
                                "totservice":totalservice})

                                
                                
# Error 404 (when page is not found)
@error(404)
def error404(error):
    return "Sorry, can't find that page!"

# Static Routes
@get('/<filename:re:.*\.js>')
def javascripts(filename):
    return static_file(filename, root='static/js')

@get('/<filename:re:.*\.css>')
def stylesheets(filename):
    return static_file(filename, root='static/css')

@get('/<filename:re:.*\.(jpg|png|gif|ico)>')
def images(filename):
    return static_file(filename, root='static/img')

@get('/<filename:re:.*\.(eot|ttf|woff|svg)>')
def fonts(filename):
    return static_file(filename, root='static/fonts')
    
@get('/<filename:re:.*\.xls>')
def dnload(filename):
   return static_file(filename, root='tmp')
  
# Re-Compile Sigma Model and Clean\Create tmp directory
clean_tmp()
compile.build(settings.sigma['model'])

# Custom Template Path and Run Server with Debug on
TEMPLATE_PATH.insert(0, "./templates/")
print "SigmaWeb Running At: http://%s:%s" % (settings.server['hostname'], settings.server['port'])

run(host=settings.server['hostname'], port=settings.server['port'], quiet=True)
