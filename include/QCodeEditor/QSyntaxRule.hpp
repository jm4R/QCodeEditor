/**
 * QCodeEditor - Widget to highlight and auto-complete code.
 * Copyright (C) 2016-2018 Nicolas Kogler
 *
 * QCodeEditor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with QCodeEditor. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef QCODEEDITOR_QSYNTAXRULE_H
#define QCODEEDITOR_QSYNTAXRULE_H

#include <QColor>
#include <QString>

#include <QCodeEditor/Config.hpp>
#include <QCodeEditor/QCodeEditorDesign.hpp>

/**
 * @author Nicolas Kogler
 * @date October 4th, 2016
 * @class QSyntaxRule
 * @brief Specifies the appearance of keywords or regular expressions.
 *
 * Under the hood, each regular expression is forwarded to a function that
 * searches for matches in the code editor's text. If there are matches, they
 * are colored and transformed as specified. Keywords will be directly converted
 * to a regular expression.
 *
 * TODO: Keywords, regex and opening/closing regex should be abstracted. (QAbstractSyntaxRule)
 */
class QCODEEDITOR_API QSyntaxRule
{
public:
    QSyntaxRule();
    QSyntaxRule(const QSyntaxRule& rule) = default;
    QSyntaxRule& operator=(const QSyntaxRule& rule) = default;
    ~QSyntaxRule() = default;

    /**
     * Retrieves the underlying regular expression.
     * @return Regular expression associated with the rule.
     */
    const QString& regex() const;

    /**
     * Retrieves the font of the matching text.
     * @returns the font in which matches are rendered.
     */
    const QFont& font() const;

    /**
     * Retrieves the background color of the matching text.
     * @return The background color of the matching text.
     */
    const QColor& backColor() const;

    /**
     * Retrieves the foreground color of the matching text.
     * @return The foreground color of the matching text.
     */
    const QColor& foreColor() const;

    /**
     * Retrieves the unique id for this rule.
     * @return The rule string identifier.
     */
    const QString& id() const;

    /**
     * Retrieves the opening regex for this multiline rule.
     * @return The opening regex.
     */
    const QString& openingRegex() const;

    /**
     * Retrieves the closing regex for this multiline rule.
     * @returns the closing regex.
     */
    const QString& closingRegex() const;

    /**
     * Determines whether the regex search should be global.
     * @return True to search globally.
     */
    bool isGlobal() const;

    /**
     * Determines whether a custom font is used.
     * @return True if a custom font is used.
     */
    bool useFont() const;

    /**
     * Specifies the regular expression for this rule.
     * @param regex Escaped regex sequence.
     */
    void setRegex(const QString& regex);

    /**
     * @brief Specifies the keywords.
     * The keywords will be automatically translated into a regular expression.
     * A usage example for 'lineBegin=true' could be preprocessor directives.
     * @param keywords List of keyword strings.
     * @param lineBegin Should keywords be at the beginning of a line?
     */
    void setKeywords(const QStringList& keywords, bool lineBegin = false);

    /**
     * Specifies the font in which matches are rendered.
     * @param font Font to use.
     */
    void setFont(const QFont& font);

    /**
     * Specifies the background color of the matches.
     * @param backColor Background color to use.
     */
    void setBackColor(const QColor& backColor);

    /**
     * Specifies the foregorund color of the matches.
     * @param foreColor Foreground color to use.
     */
    void setForeColor(const QColor& foreColor);

    /**
     * Specifies the identifier for this rule.
     * @param id String identifier for this rule.
     */
    void setId(const QString& id);

    /**
     * Specifies the opening regular expression for this rule.
     * @param regex Escaped regex sequence.
     */
    void setOpeningRegex(const QString& regex);

    /**
     * Specifies the closing regular expression for this rule.
     * @param regex Escaped regex sequence.
     */
    void setClosingRegex(const QString& regex);

    /**
     * Specifies whether the regex search should be global.
     * @param global True to perform a global regex search.
     */
    void setGlobal(bool global);

private:
    QString _regex;
    QFont _font;
    QColor _backColor;
    QColor _foreColor;
    QString _id;
    QString _startReg;
    QString _endReg;
    bool _isGlobal;
    bool _useFont;

};


/**
 * @author Nicolas Kogler
 * @date October 7th, 2016
 * @class QSyntaxRules
 */
class QCODEEDITOR_API QSyntaxRules {
public:
    /**
     * Loads a list of rules from a XML file.
     * @param path Absolute path or resource path.
     * @param design Current editor design (need the default font)
     */
    static QList<QSyntaxRule> loadFromFile(const QString& path, const QCodeEditorDesign& design);
};


#endif
