# Server code

All the code used to run the server side of this project is contained in the /src/ folder.

## xml

### Wemos

#### Signup
Request:
```xml
<message>
    <header>
        <sender>wemosNaam</sender>
        <receiver>server</receiver>
    </header>
    <function>authentication</function>
    <context>
        <password>solarwinds123</password>
        <clientName>Keuken Lamp</clientName>
        <type>lamp</type>
    </context>
</message>
```

Acknowledgement:
```xml
<message>
    <header>
        <sender>server</sender>
        <receiver>wemosNaam</receiver>
    </header>
    <function>ack</function>
    <context></context>
</message>
```

status update:
```xml
<message>
    <header>
        <sender>Wemosnaam</sender>
        <receiver>server</receiver>
    </header>
    <function>sensorUpdate</function>
    <context>
        <type>x</type>    
        <data1>0</data1>
        <data2>0</data2>  
    </context>
</message>
```

actuateBool

```xml
<message>
    <header>
        <sender>server</sender>
        <receiver>wemosnaam</receiver>
    </header>
    <function>actuateBool</function>
    <context>
        <data1>0</data1> 
        <data2>0</data2> 
    </context>
</message>
```

#### Get status broadcast
Request:
```xml
<message>
    <header>
        <sender>server</sender>
        <receiver>allWemos</receiver>
    </header>
    <function>getStatusBroadcast</function>
    <context>
        <password>solarwinds123</password>
    </context>
</message>
```

reply:
```xml
<message>
    <header>
        <sender>wemosNaam</sender>
        <receiver>server</receiver>
    </header>
    <function>answerToStatusRequest</function>
    <context>
        <type>lamp</type>
        <data>1</data>
    </context>
</message>
```


### Site

#### Signup
Request:
```xml
<message>
    <header>
        <sender>website</sender>
        <receiver>server</receiver>
    </header>
    <function>authentication</function>
    <context>
        <password>solarwinds123</password>
        <clientName>Website</clientName>
        <type>website</type>
    </context>
</message>
```

Acknowledgement:
```xml
<message>
    <header>
        <sender>server</sender>
        <receiver>website</receiver>
    </header>
    <function>ack</function>
    <context></context>
</message>
```

#### All statusses
Request:
```xml
<message>
    <header>
        <sender>website</sender>
        <receiver>server</receiver>
    </header>
    <function>getStatusAll</function>
    <context>
        <password>solarwinds123</password>
    </context>
</message>
```

Reply:
```xml
<message>
    <header>
        <sender>server</sender>
        <receiver>website</receiver>
    </header>
    <function>answerToStatusRequest</function>
    <context>
        <wemosjes>
            <wemos>
                <name>UniekeWemosNaam</name>
                <type>stoel</type>
                <data1>0</data1>
            </wemos>
            <wemos>
                <name>AndereUniekeWemosNaam</name>
                <type>lamp</type>
                <data1>1</data1>
            </wemos>
        </wemosjes>
    </context>
</message>
```

#### Toggle functions wemos
Request:
```xml
<message>
	<header>
		<sender>website</sender>
		<receiver>server</receiver>
	</header>
	<function>changeStatus</function>
	<context>
		<password>solarwinds123</password>
        <clientName>UniekeWemosNaam</clientName>
        <type>lamp</type>
        <data1>1</data1>
	</context>
</message>
```

reply:
```xml
<unknown/>
```

server to selected wemos:
```xml
<message>
    <header>
        <sender>server</sender>
        <receiver>wemosnaam</receiver>
    </header>
    <function>actuateBool</function>
    <context>
        <data1>0</data1> 
        <data2>0</data2>
	etc...
    </context>
</message>
```