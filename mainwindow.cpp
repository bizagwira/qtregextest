#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRegularExpression>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString
MainWindow::match_pattern_substr(const QString text, const QString regex_pattern){
    QRegularExpression regex_matcher(regex_pattern,
                                  QRegularExpression::CaseInsensitiveOption);
    QString match_substr = QString();
    QRegularExpressionMatchIterator i = regex_matcher.globalMatch(text);
    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        if (match.hasMatch()) {
             match_substr = match.captured(0);
        }
    }

    return match_substr;
}

QString
MainWindow::substr_extracter(QString str, const QString pattern){
    QRegExp rx(pattern);
    QList<QString> list;
    int pos = 0;
    while ((pos = rx.indexIn(str, pos)) != -1) {
        list << rx.cap(1);
        pos += rx.matchedLength();
    }
    if (!list.isEmpty()){
        return list.at(0);
    }
    return QString();
}

QString
MainWindow::search_and_extract(const QString input_text, const QString regex_pattern){
    const QString searching_pattern = QString("%1\\S*").arg(regex_pattern);
//    qDebug() << _regex_pattern<<"\t" << searching_pattern;
    QString match_substr = match_pattern_substr(input_text, searching_pattern);
//    qDebug() << match_substr;
    QString text_str;
    if (!match_substr.isEmpty()){
        text_str =substr_extracter(match_substr, regex_pattern);
    }
    return text_str;
}

void
MainWindow::on_quit_button_clicked(){
    qApp->exit(0);
}

void
MainWindow::on_search_button_clicked(){
    QString text_str = match_pattern_substr(_input_text, _search_regex);
    ui->console_pte->appendPlainText(text_str);
}

void
MainWindow::on_extract_button_clicked(){
    _input_text = ui->input_text_led->text();
    _extract_regex = ui->extract_regex_led->text();
    int p = progress(_input_text);
    QString pstr = QString("Progress: %1").arg(p);
    ui->console_pte->appendPlainText(pstr);
//    qDebug() << "Progress: " << p;
    QString text_str = search_and_extract(_input_text, _extract_regex);
    ui->console_pte->appendPlainText(text_str);
}

void
MainWindow::on_input_text_led_textChanged(const QString &input_text){
    _input_text = input_text;
}

void
MainWindow::on_search_regex_led_textChanged(const QString &search_regex){
    _search_regex = search_regex;
}

void
MainWindow::on_extract_regex_led_textChanged(const QString &extract_regex){
    _extract_regex = extract_regex;
}

void
MainWindow::on_replace_button_clicked(){
    QString text = ui->input_text_led->text();
    QString replace_by_text = ui->replace_by_text_led->text();
    text = text.replace(QRegExp(_replace_regex), replace_by_text);
    ui->console_pte->appendPlainText(text);
}

void
MainWindow::on_replace_regex_led_textChanged(const QString &replace_regex ){
    _replace_regex = replace_regex;
}

void
MainWindow::on_apply_button_clicked(){
    QStringList command_list;
    command_list.append("*RST\r");
    command_list.append("TRAC:CLE");
    command_list.append("INIT:CONT OFF");
    command_list.append("ROUT:OPEN:ALL");
    command_list.append(":TRIG:COUN 1");
    command_list.append(":SENS:RES:RANGe:AUTO OFF");
    command_list.append(":SENS:RES:RANGe 0.002\r");
    QMutableStringListIterator i(command_list); // pass list as argument
    while (i.hasNext()) {
        QString cmd = i.next();                  // retrieve value of current item
        cmd = cmd.replace(QRegExp("(\\w*RANGe)\\s*(\\d+\\.\\d+)(\\r)$"), QString("\\1 %1\\3").arg(0.200));
        ui->console_pte->appendPlainText(cmd);
    }
    ui->console_pte->appendPlainText("============================");
    foreach (const QString &cmd, command_list) {
        ui->console_pte->appendPlainText(cmd);
    }
}


bool
MainWindow::find(const QString text, const QString pattern){
    QRegularExpression re(pattern, QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch match = re.match(text);
    return match.hasMatch();
}

int
MainWindow::progress(const QString text){
    if(find(text, "([x0-9a-fA-F]{10}[\.]*\\s*\\(\\s*\\d+\\s*%\\))")){
//    if(find(text, "[x0-9a-fA-F]{10}[\.]*\\s*\(\\s*\\d+\\s*%\)")){
        QRegExp rx("(\\d+)\\s*%");
        QList<QString> list;
        int pos = 0;
        while ((pos = rx.indexIn(text, pos)) != -1) {
            list << rx.cap(1);
            pos += rx.matchedLength();
        }

        if (!list.isEmpty()){
            return list.at(0).toInt();
        }
    }
    return  0;
}


template<typename T>
T extract(const QString text, const QString pattern){
    QRegExp rx(pattern);
    QList<QString> list;
    int pos = 0;
    while ((pos = rx.indexIn(text, pos)) != -1) {
        list << rx.cap(1);
        pos += rx.matchedLength();
    }
    if (!list.isEmpty()){
        return list.at(0);
    }
    return QString();
}
