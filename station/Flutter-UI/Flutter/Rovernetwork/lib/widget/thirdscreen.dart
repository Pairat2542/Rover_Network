import 'package:flutter/material.dart';
import 'package:flutter/src/widgets/basic.dart';
import 'package:flutter/src/widgets/container.dart';
import 'package:flutter/src/widgets/container.dart';
import 'package:flutter/src/widgets/container.dart';
import 'package:google_fonts/google_fonts.dart';
import 'dart:async';
import 'dart:convert';
import 'package:http/http.dart' as http;
import 'dart:io';

import 'firstscreen.dart';
import 'fourscreen.dart';

class ThirdScreen extends StatefulWidget {
  @override
  _ThirdScreenScreenState createState() => _ThirdScreenScreenState();
}

class _ThirdScreenScreenState extends State<ThirdScreen> {
  @override
  void initState() {
    super.initState();
    _startTimer();
  }

  Timer _timer;

  void _startTimer() {
    _timer = Timer.periodic(Duration(seconds: 1), (timer) {
      setState(() {
        http.get('http://192.168.43.2:8080/').then((response) {
          if (response.statusCode == 200) {
            var resJson = json.decode(response.body);
            stdout.writeln(resJson);
            if (resJson["status"] == true && resJson["state"] == 3) {
              timer.cancel();
              Navigator.pushReplacement(context,
                  MaterialPageRoute(builder: (context) => FourScreen()));
            }
          } else {
            throw Exception('Failed to load album');
          }
        });
      });
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text("ROVER NETWORK PROJECT"),
        backgroundColor: Colors.blueAccent,
      ),
      body: Container(
        decoration: BoxDecoration(
          gradient: RadialGradient(
            center: Alignment(0, -0.5),
            radius: 1,
            colors: [Colors.white, Colors.blue],
          ),
        ),
        child: Center(
          child: Column(
            mainAxisSize: MainAxisSize.max,
            children: [
              buildContainer(),
              buildText(),
              CircularProgressIndicator()
            ],
          ),
        ),
      ),
    );
  }

//chakraPetch is work google font
  Container buildText() => Container(
        margin: EdgeInsets.only(bottom: 50),
        child: Text(
          "CAR IS COMING !!",
          style: GoogleFonts.raleway(
            textStyle: TextStyle(
              fontSize: 35,
              fontWeight: FontWeight.bold,
              color: Colors.red[700],
            ),
          ),
        ),
      );

  Container buildContainer() {
    return Container(
      margin: EdgeInsets.only(top: 10),
      width: 180,
      child: Image.asset('images/logo.png'),
    );
  }
}
