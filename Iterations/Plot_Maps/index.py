import pandas as pd
import folium

# read excel data as dataframe
dataDf = pd.read_excel('emergency_data.xlsx')

# initialize a map with center and zoom
mapObj = folium.Map(location=[11.499785, 75.688642],
                     zoom_start=16, tiles='openstreetmap')


# create a layer for bubble map using FeatureGroup
emergencyDataLayer = folium.FeatureGroup("Emergency Data")
# add the created layer to the map
emergencyDataLayer.add_to(mapObj)

# iterate through each dataframe row
for i in range(len(dataDf)):
    name = dataDf.iloc[i]['name']
    location = dataDf.iloc[i]['location']
    medical = dataDf.iloc[i]['medical']
    fire = dataDf.iloc[i]['fire']
    collapse = dataDf.iloc[i]['collapse']
    Message = dataDf.iloc[i]['message']
   
    # set circle color
    clr = "red"
    # set circle radius
    radius = 30
    # Derive popup HTML content
    popUpStr = 'Name - {0}<br>Location - {1}<br>Needs Medical Attention? - {2}<br>Fire Hazard? - {3}<br>Buillding Collapse? - {4}<br>Message: - {5} MW'.format(
        name, location, medical, fire, collapse, Message)
    # draw a circle for the emergency reported location
    folium.Circle(
        location=[dataDf.iloc[i]['lat'], dataDf.iloc[i]['lng']],
        popup=folium.Popup(popUpStr, min_width=100, max_width=700),
        radius=radius,
        color=clr,
        weight=2,
        fill=True,
        fill_color=clr,
        fill_opacity=0.1
    ).add_to(emergencyDataLayer)


# add layer control over the map
folium.LayerControl().add_to(mapObj)

# html to be injected for displaying legend
legendHtml = '''
     <div style="position: fixed; 
     bottom: 50px; left: 50px; width: 150px; height: 70px; 
     border:2px solid grey; z-index:9999; font-size:14px;
     ">&nbsp; Legend <br>
     &nbsp; <i class="fa fa-circle"
                  style="color:red"></i> &nbsp; Emergency<br>
      </div>
     '''

# inject html corresponding to the legend into the map
mapObj.get_root().html.add_child(folium.Element(legendHtml))

# save the map as html file
mapObj.save('output.html')