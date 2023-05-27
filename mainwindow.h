#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <QImage>
#include <QPainter>

class MainWindow : public QMainWindow
{
	Q_OBJECT
	QFont f;
	QColor c_tx = QColor::fromRgb(0, 0, 0),
		c_ol = QColor::fromRgb(255, 255, 255),
		c_bg = QColor::fromRgb(128, 128, 128);
	double w_ol = 5, w_pl = 25, f_sz = 10;
	QString s = "Qt-based generator for text telegram stickers";
	QImage img, im_text, im_c_tx, im_c_ol, im_col_tx, im_ol, im_col_ol, im_empty;
	QPainter::CompositionMode compmd;

public:
	explicit MainWindow(QWidget *parent = 0);

public:
	void paintEvent(QPaintEvent *event);

public slots:
	void on_action_Save_triggered();
	void on_action_Set_text_triggered();
	void on_action_Set_font_triggered();
	void on_action_Set_back_colors_triggered();
	void on_action_Set_text_colors_triggered();
	void on_action_Set_outline_colors_triggered();
	void on_action_Set_pollia_triggered();
	void on_action_Set_outline_triggered();
	void on_action_About_Qt_triggered();
	void on_action_About_triggered();

public:
	void calculate_font_size(QPainter *p);
	bool font_size_fits(QPainter *p, double d);

public:
	enum EUpdMode { None = 0, Text, Color_Text, Compose_Text, Size_Outline, Color_Outline, Compose_Outline, Compose, All };
	void update_pic(EUpdMode md);
	void update_pic_2(EUpdMode md);

private:
	Ui::MainWindow ui;
};

#endif // MAINWINDOW_H
