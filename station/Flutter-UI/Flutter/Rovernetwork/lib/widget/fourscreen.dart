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

class FourScreen extends StatefulWidget {
  @override
  _FourScreenScreenState createState() => _FourScreenScreenState();
}

class _FourScreenScreenState extends State<FourScreen> {
  @override
  void initState() {
    super.initState();
    _startTimer();
  }

  Timer _timer;

  void _startTimer() {
    _timer = Timer.periodic(Duration(seconds: 3), (timer) {
      setState(() {
        http.get('http://192.168.43.2:8080/refresh').then((response) {
          if (response.statusCode == 200) {
            var resJson = json.decode(response.body);
            stdout.writeln(resJson);
            if (resJson["status"] == true && resJson["state"] == 3) {
              timer.cancel();
              Navigator.pushReplacement(context,
                  MaterialPageRoute(builder: (context) => MainScreen()));
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
            center: Alignment(0, -0.4),
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
              buildRaisedButton(),
            ],
          ),
        ),
      ),
    );
  }

  Container buildRaisedButton() => Container(
        width: 200,
        height: 50,
        margin: EdgeInsets.only(top: 20),
        child: RaisedButton(
          color: Colors.orange[300],
          onPressed: () {
            http.get('http://192.168.43.2:8080/refresh').then((response) {
              if (response.statusCode == 200) {
                Navigator.pushReplacement(context,
                    MaterialPageRoute(builder: (context) => MainScreen()));
              } else {
                throw Exception('Failed to load album');
              }
            });
          },
          child: Text("EXIT",
              style: GoogleFonts.raleway(
                textStyle: TextStyle(
                  fontSize: 20,
                  fontWeight: FontWeight.bold,
                  color: Colors.white,
                ),
              )),
          shape:
              RoundedRectangleBorder(borderRadius: BorderRadius.circular(20)),
        ),
      );

  Container buildText() => Container(
        margin: EdgeInsets.only(bottom: 10),
        child: Text(
          "CAR IS ARRIVED !!",
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
