import 'dart:io';
import 'package:path_provider/path_provider.dart';
import 'package:share_plus/share_plus.dart';

class ExportService {
  /// Writes telemetry log rows to a local CSV file
  static Future<File> writeLogToCSV(List<String> logData) async {
    final directory = await getApplicationDocumentsDirectory();
    final timestamp = DateTime.now().millisecondsSinceEpoch;
    final filePath = '${directory.path}/matrisense_telemetry_$timestamp.csv';
    
    final file = File(filePath);
    // Join lines with standard CSV newline breaks
    return await file.writeAsString(logData.join('\n'));
  }

  /// Triggers native OS share sheet (WhatsApp, Email, Drive, etc.) for the CSV log
  static Future<void> shareCSVLog(List<String> logData) async {
    if (logData.length <= 1) {
      throw Exception("No telemetry data collected yet to export.");
    }

    final file = await writeLogToCSV(logData);
    
    // Launch native sharing dialog
    await Share.shareXFiles(
      [XFile(file.path)],
      text: 'MatriSense Wearable Telemetry Data Export (${DateTime.now().toIso8601String()})',
      subject: 'MatriSense Vitals Telemetry CSV',
    );
  }
}
