#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private:
        QString match_pattern_substr(const QString text, const QString regex_pattern);

        QString substr_extracter(QString str, const QString pattern);

        QString search_and_extract(const QString input_text, const QString regex_pattern);

    private slots:
        void on_quit_button_clicked();

        void on_search_button_clicked();

        void on_extract_button_clicked();

        void on_input_text_led_textChanged(const QString &input_text);

        void on_search_regex_led_textChanged(const QString &search_regex);

        void on_extract_regex_led_textChanged(const QString &extract_regex);

        void on_replace_button_clicked();

        void on_replace_regex_led_textChanged(const QString &arg1);

        void on_apply_button_clicked();

    private:
        bool find(const QString text, const QString pattern);
        int progress(const QString text);



    private:
        Ui::MainWindow *ui;
        QString _search_regex, _extract_regex, _replace_regex, _input_text;
};

template <typename T>
T extract(const QString text, const QString pattern);
#endif // MAINWINDOW_H
