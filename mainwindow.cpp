#include "mainwindow.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QPaintEngine>
#include <QFontDialog>
#include <QColorDialog>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, img(512, 512, QImage::Format_ARGB32)
	, im_text(512, 512, QImage::Format_ARGB32)
	, im_c_tx(512, 512, QImage::Format_ARGB32)
	, im_c_ol(512, 512, QImage::Format_ARGB32)
	, im_col_tx(512, 512, QImage::Format_ARGB32)
	, im_ol(512, 512, QImage::Format_ARGB32)
	, im_col_ol(512, 512, QImage::Format_ARGB32)
	, im_empty(512, 512, QImage::Format_ARGB32)
{
	ui.setupUi(this);
	QPainter p;
	p.begin(&img);
	compmd = p.compositionMode();
	p.end();
	update_pic(All);
}

void MainWindow::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);
	QPainter p(this);
	p.setCompositionMode(compmd);
	p.fillRect(rect(), c_bg);
	p.drawImage(0, 0, img);
}

void MainWindow::on_action_Save_triggered()
{
	QString fn = QFileDialog::getSaveFileName(this, "Save image", "", "PNG image (*.png);;All Files (*.*)");
	if(!fn.isEmpty())
	{
		if(!img.save(fn))
		{
			QMessageBox::critical(this, windowTitle(), fn + "\nCan't save to file");
		}
	}
}

void MainWindow::on_action_Set_text_triggered()
{
	bool ok;
	QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
		tr("Text to be displayed:"), QLineEdit::Normal,
		s, &ok);
	if (ok && !text.isEmpty())
	{
		s = text;
		update_pic(Text);
	}
}

void MainWindow::on_action_Set_font_triggered()
{
	auto old = f;
	f = QFontDialog::getFont(nullptr, f, this, "Select font");
	if (old != f)
		update_pic(Text);
}


void MainWindow::on_action_Set_back_colors_triggered()
{
	auto old = c_bg;
	c_bg = QColorDialog::getColor(c_bg, this, "Select background color");
	if (c_bg.isValid())
		update_pic(Compose);
	else
		c_bg = old;
}

void MainWindow::on_action_Set_text_colors_triggered()
{
	auto old = c_tx;
	c_tx = QColorDialog::getColor(c_tx, this, "Select background color");
	if (c_tx.isValid())
		update_pic(Color_Text);
	else
		c_tx = old;
}

void MainWindow::on_action_Set_outline_colors_triggered()
{
	auto old = c_ol;
	c_ol = QColorDialog::getColor(c_ol, this, "Select background color");
	if (c_ol.isValid())
		update_pic(Color_Outline);
	else
		c_ol = old;
}

void MainWindow::on_action_Set_pollia_triggered()
{
	auto old = w_pl;
	w_pl = QInputDialog::getDouble(this, windowTitle(), "Pollia for text (0-200):", w_pl, 0, 200);
	if (old != w_pl)
		update_pic(Text);
}

void MainWindow::on_action_Set_outline_triggered()
{
	auto old = w_ol;
	w_ol = QInputDialog::getDouble(this, windowTitle(), "Outline for text (0-50):", w_ol, 0, 50);
	if (old != w_ol)
		update_pic(Size_Outline);
}

void MainWindow::on_action_About_Qt_triggered()
{
	qApp->aboutQt();
}

void MainWindow::on_action_About_triggered()
{
	QMessageBox::about(this, windowTitle(),
		"QTgTSG, version 1.0\n"
		"Qt Telegram Text Sticker Generator\n"
		"2020 [redacted]\n");
}

void MainWindow::calculate_font_size(QPainter *p)
{
	double le = 1, ri = 1000, mid;
	for(int i = 0; i < 100; ++i)
	{
		mid = le + (ri - le) / 2;
		if(font_size_fits(p, mid))
			le = mid;
		else
			ri = mid;
	}
	f_sz = mid;
}

bool MainWindow::font_size_fits(QPainter *p, double d)
{
	f.setPointSizeF(d);
	p->setFont(f);
	QRectF rc(QPointF(w_pl, w_pl), QSizeF(512 - 2 * w_pl, 512 - 2 * w_pl));
	rc = p->boundingRect(rc, Qt::AlignCenter | Qt::TextWordWrap, s);
	return rc.height() <= 512 - w_pl && rc.width() <= 512 - w_pl;
}

void MainWindow::update_pic(EUpdMode md)
{
	switch(md)
	{
		case None:
			break;
		case Text:
			update_pic_2(Text);
			update_pic_2(Compose_Text);
			update_pic_2(Size_Outline);
			update_pic_2(Color_Outline);
			update_pic_2(Compose_Outline);
			update_pic_2(Compose);
			break;
		case Color_Text:
			update_pic_2(Color_Text);
			update_pic_2(Compose_Text);
			update_pic_2(Compose_Outline);
			update_pic_2(Compose);
			break;
		case Compose_Text:
			update_pic_2(Compose_Text);
			update_pic_2(Compose);
			break;
		case Size_Outline:
			update_pic_2(Size_Outline);
			update_pic_2(Compose_Outline);
			update_pic_2(Compose);
			break;
		case Color_Outline:
			update_pic_2(Color_Outline);
			update_pic_2(Compose_Outline);
			update_pic_2(Compose);
			break;
		case Compose_Outline:
			update_pic_2(Compose_Outline);
			update_pic_2(Compose);
			break;
		case Compose:
			update_pic_2(Compose);
			break;
		case All:
			update_pic_2(Text);
			update_pic_2(Color_Text);
			update_pic_2(Compose_Text);
			update_pic_2(Size_Outline);
			update_pic_2(Color_Outline);
			update_pic_2(Compose_Outline);
			update_pic_2(Compose);
			break;
		default:
			break;
	}
	update();
}

void MainWindow::update_pic_2(EUpdMode md)
{
	QPainter p;
	switch(md)
	{
		case Text:
			// -> im_text
			im_text = im_empty;
			p.begin(&im_text);
			p.fillRect(im_text.rect(), Qt::transparent);
			calculate_font_size(&p);
			f.setPixelSize(f_sz);
			p.setFont(f);
			p.setPen(QColor::fromRgb(255, 255, 255, 255));
			p.drawText(QRectF(QPointF(w_pl, w_pl), QSizeF(512 - 2 * w_pl, 512 - 2 * w_pl)),
				Qt::AlignCenter | Qt::TextWordWrap, s);
			break;
		case Color_Text:
			// -> im_c_tx
			p.begin(&im_c_tx);
			p.fillRect(im_c_tx.rect(), c_tx);
			break;
		case Compose_Text:
			// im_text, im_c_tx -> im_col_tx
			im_col_tx = im_empty;
			p.begin(&im_col_tx);
			p.fillRect(im_col_tx.rect(), Qt::transparent);
			p.setCompositionMode(QPainter::CompositionMode_Source);
			p.drawImage(0, 0, im_c_tx);
			p.setCompositionMode(QPainter::CompositionMode_DestinationIn);
			p.drawImage(0, 0, im_text);
			break;
		case Size_Outline:
			// im_text -> im_ol
			im_ol = im_empty;
			p.begin(&im_ol);
			p.fillRect(im_ol.rect(), Qt::transparent);
			p.setPen(QColor::fromRgb(255, 255, 255, 255));
			for(int X = 0, A2 = w_ol * w_ol; X <= w_ol; ++X)
			{
				int X2 = X*X;
				for(int Y = X; Y <= w_ol; ++Y)
				{
					if(X2 + Y*Y > A2)
						break;
					p.drawImage(+X, +Y, im_text);
					p.drawImage(+X, -Y, im_text);
					p.drawImage(-X, +Y, im_text);
					p.drawImage(-X, -Y, im_text);
					p.drawImage(+Y, +X, im_text);
					p.drawImage(+Y, -X, im_text);
					p.drawImage(-Y, +X, im_text);
					p.drawImage(-Y, -X, im_text);
				}
			}
			break;
		case Color_Outline:
			// -> im_c_ol
			im_c_ol = im_empty;
			p.begin(&im_c_ol);
			p.fillRect(im_c_ol.rect(), c_ol);
			break;
		case Compose_Outline:
			// im_ol, im_c_ol -> im_col_ol
			im_col_ol = im_empty;
			p.begin(&im_col_ol);
			p.fillRect(im_col_ol.rect(), Qt::transparent);
			p.setCompositionMode(QPainter::CompositionMode_Source);
			p.drawImage(0, 0, im_c_ol);
			p.setCompositionMode(QPainter::CompositionMode_DestinationIn);
			p.drawImage(0, 0, im_ol);
			break;
		case Compose:
			// im_col_tx, im_col_ol -> img
			img = im_empty;
			p.begin(&img);
			p.fillRect(img.rect(), Qt::transparent);
			p.setCompositionMode(QPainter::CompositionMode_Source);
			p.drawImage(0, 0, im_col_tx);
			p.setCompositionMode(QPainter::CompositionMode_DestinationOver);
			p.drawImage(0, 0, im_col_ol);
			break;
		default:
			break;
	}
	p.end();
}

