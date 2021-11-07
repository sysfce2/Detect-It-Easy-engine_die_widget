// copyright (c) 2019-2021 hors<horsicq@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#include "die_highlighter.h"

DIE_Highlighter::DIE_Highlighter(QObject *pParent) : QSyntaxHighlighter(pParent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns << "\\bvar\\b"
                    << "\\bfunction\\b"
                    << "\\bbShowType\\b"
                    << "\\bbShowVersion\\b"
                    << "\\bbShowOptions\\b"
                    << "\\bsVersion\\b"
                    << "\\bsOptions\\b"
                    << "\\sName\\b"
                    << "\\bbDetected\\b"
                    << "\\breturn\\b";

    foreach(const QString &pattern, keywordPatterns)
    {
        rule.pattern=QRegExp(pattern);
        rule.format=keywordFormat;
        listHighlightingRules.append(rule);
    }

    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern=QRegExp("\\bQ[A-Za-z]+\\b");
    rule.format=classFormat;
    listHighlightingRules.append(rule);

    singleLineCommentFormat.setForeground(Qt::darkGreen);
    rule.pattern=QRegExp("//[^\n]*");
    rule.format=singleLineCommentFormat;
    listHighlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::darkGreen);

    quotationFormat.setForeground(Qt::darkGreen);
    //          rule.pattern=QRegExp("\".*\"");
    rule.pattern=QRegExp("\"[^\"]+\"");
    rule.format=quotationFormat;
    listHighlightingRules.append(rule);

    //          functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.pattern=QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format=functionFormat;
    listHighlightingRules.append(rule);

    commentStartExpression=QRegExp("/\\*");
    commentEndExpression=QRegExp("\\*/");
}

void DIE_Highlighter::highlightBlock(const QString &text)
{
    foreach(const HighlightingRule &rule, listHighlightingRules)
    {
        QRegExp expression(rule.pattern);
        qint32 nIndex=expression.indexIn(text);

        while(nIndex>=0)
        {
            qint32 length=expression.matchedLength();
            setFormat(nIndex,length,rule.format);
            nIndex=expression.indexIn(text,nIndex+length);
        }
    }

    setCurrentBlockState(0);

    qint32 nStartIndex=0;

    if(previousBlockState()!=1)
    {
        nStartIndex=commentStartExpression.indexIn(text);
    }

    while(nStartIndex>=0)
    {
        qint32 nEndIndex=commentEndExpression.indexIn(text,nStartIndex);
        qint32 nCommentLength=0;

        if(nEndIndex==-1)
        {
            setCurrentBlockState(1);
            nCommentLength=text.length()-nStartIndex;
        }
        else
        {
            nCommentLength=nEndIndex-nStartIndex+commentEndExpression.matchedLength();
        }

        setFormat(nStartIndex,nCommentLength,multiLineCommentFormat);
        nStartIndex=commentStartExpression.indexIn(text,nStartIndex+nCommentLength);
    }
}
