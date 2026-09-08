import 'dart:async';
import 'package:flutter/material.dart';
import 'ble_controller.dart';
import 'export_service.dart';

void main() {
  runApp(const MatriSenseApp());
}

class MatriSenseApp extends StatelessWidget {
  const MatriSenseApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'MatriSense Monitor',
      debugShowCheckedModeBanner: false,
      theme: ThemeData(
        colorScheme: ColorScheme.fromSeed(seedColor: Colors.teal),
        useMaterial3: true,
      ),
      home: const DashboardScreen(),
    );
  }
}

class DashboardScreen extends StatefulWidget {
  const DashboardScreen({super.key});

  @override
  State<DashboardScreen> createState() => _DashboardScreenState();
}

class _DashboardScreenState extends State<DashboardScreen> {
  final BleController _bleController = BleController();
  StreamSubscription? _vitalsSubscription;
  bool _isConnected = false;

  // Real-time Vitals State
  int _heartRate = 0;
  int _spO2 = 0;
  double _temperature = 0.0;
  double _accelX = 0.0, _accelY = 0.0, _accelZ = 0.0;
  int _alertStatus = 0; // 0: Normal, 1: Caution, 2: Critical

  @override
  void initState() {
    super.initState();
    _vitalsSubscription = _bleController.vitalsStream.listen((vitals) {
      setState(() {
        _heartRate = vitals['hr'];
        _spO2 = vitals['spo2'];
        _temperature = vitals['temp'];
        _accelX = vitals['ax'];
        _accelY = vitals['ay'];
        _accelZ = vitals['az'];
        _alertStatus = vitals['status'];
        _isConnected = true;
      });
    });
  }

  @override
  void dispose() {
    _vitalsSubscription?.cancel();
    _bleController.dispose();
    super.dispose();
  }

  Color _getAlertColor() {
    switch (_alertStatus) {
      case 2:
        return Colors.red.shade600;
      case 1:
        return Colors.amber.shade700;
      default:
        return Colors.teal.shade600;
    }
  }

  String _getAlertText() {
    switch (_alertStatus) {
      case 2:
        return "CRITICAL ALERT";
      case 1:
        return "CAUTION / WARNING";
      default:
        return "NORMAL / SAFE";
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('MatriSense Node Monitor'),
        backgroundColor: _getAlertColor(),
        foregroundColor: Colors.white,
        actions: [
          IconButton(
            icon: const Icon(Icons.share_outlined),
            tooltip: 'Share CSV Telemetry',
            onPressed: () async {
              try {
                await ExportService.shareCSVLog(_bleController.logData);
              } catch (e) {
                if (mounted) {
                  ScaffoldMessenger.of(context).showSnackBar(
                    SnackBar(
                      content: Text('Export failed: ${e.toString()}'),
                      backgroundColor: Colors.red,
                    ),
                  );
                }
              }
            },
          )
        ],
      ),
      body: SingleChildScrollView(
        padding: const EdgeInsets.all(16.0),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.stretch,
          children: [
            // Status Banner
            Container(
              padding: const EdgeInsets.all(12.0),
              decoration: BoxDecoration(
                color: _getAlertColor().withOpacity(0.15),
                borderRadius: BorderRadius.circular(8.0),
                border: Border.all(color: _getAlertColor(), width: 2),
              ),
              child: Row(
                mainAxisAlignment: MainAxisAlignment.spaceBetween,
                children: [
                  Text(
                    _getAlertText(),
                    style: TextStyle(
                      fontWeight: FontWeight.bold,
                      color: _getAlertColor(),
                      fontSize: 16,
                    ),
                  ),
                  Icon(
                    _isConnected
                        ? Icons.bluetooth_connected
                        : Icons.bluetooth_searching,
                    color: _getAlertColor(),
                  ),
                ],
              ),
            ),
            const SizedBox(height: 20),

            // Bluetooth Scan / Reconnect Action Button
            ElevatedButton.icon(
              style: ElevatedButton.styleFrom(
                padding: const EdgeInsets.all(14.0),
                backgroundColor: _getAlertColor(),
                foregroundColor: Colors.white,
              ),
              onPressed: () => _bleController.startScanAndConnect(),
              icon: const Icon(Icons.search),
              label: Text(
                _isConnected ? 'Re-Scan Bluetooth' : 'Connect to MatriSense',
              ),
            ),
            const SizedBox(height: 20),

            // Live Vitals Metric Display Grid
            GridView.count(
              crossAxisCount: 2,
              shrinkWrap: true,
              physics: const NeverScrollableScrollPhysics(),
              crossAxisSpacing: 12,
              mainAxisSpacing: 12,
              children: [
                _buildMetricCard(
                  "Heart Rate",
                  "$_heartRate BPM",
                  Icons.favorite,
                  Colors.redAccent,
                ),
                _buildMetricCard(
                  "SpO2",
                  "$_spO2 %",
                  Icons.water_drop,
                  Colors.blueAccent,
                ),
                _buildMetricCard(
                  "Skin Temp",
                  "${_temperature.toStringAsFixed(2)} °C",
                  Icons.thermostat,
                  Colors.orangeAccent,
                ),
                _buildMetricCard(
                  "Motion (X,Y,Z)",
                  "${_accelX.toStringAsFixed(1)}, ${_accelY.toStringAsFixed(1)}, ${_accelZ.toStringAsFixed(1)}",
                  Icons.vibration,
                  Colors.purpleAccent,
                ),
              ],
            ),
          ],
        ),
      ),
    );
  }

  Widget _buildMetricCard(
    String title,
    String value,
    IconData icon,
    Color color,
  ) {
    return Card(
      elevation: 2,
      shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(12)),
      child: Padding(
        padding: const EdgeInsets.all(12.0),
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            Icon(icon, size: 32, color: color),
            const SizedBox(height: 8),
            Text(
              title,
              style: const TextStyle(fontSize: 12, color: Colors.grey),
            ),
            const SizedBox(height: 4),
            Text(
              value,
              style: const TextStyle(
                fontSize: 16,
                fontWeight: FontWeight.bold,
              ),
            ),
          ],
        ),
      ),
    );
  }
}
