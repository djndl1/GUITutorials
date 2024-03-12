#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

/*!
 * \namespace Ui
 * \brief the implementation namespace, where the actual Ui classes are generated from .ui files
 */
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/*!
 * \brief The MainWindow class, a NotePad window
 */
class MainWindow : public QMainWindow
{
    //! required by every QObject
    Q_OBJECT

public:
    /*!
     * \brief MainWindow
     * \param parent the parent widget; without which it is a top-level window
     */
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void newDocument();

    void open();

    void save();

    void saveAs();

    void print();

    void exit();

    void copy();

    void cut();

    void paste();

    void undo();

    void redo();

    void selectFont();

    void setFontBold(bool bold);

    void setFontUnderline(bool underlined);

    void setFontItalic(bool italic);

    void about();


private:
    // the pointer to implementation
    Ui::MainWindow *ui;

    QString currentFile;
};
#endif // MAINWINDOW_H
