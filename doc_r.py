import sys
import docx
import codecs

# Função para leitura de texto de um .docx
def getText(filename):
    doc = docx.Document(filename)
    fulltext =[]
    for t in doc.paragraphs:
        for run in t.runs:
            if run.italic:                                                              # Identify italic text
                t.text = t.text.replace(run.text, '\\textit{'+run.text+'}')
            elif run.bold:                                                              # Identify Bold text
                t.text = t.text.replace(run.text, '\\textbf{'+run.text+'}')
            elif run.font.superscript:                                                  # Identify superscript text
                t.text = t.text.replace(run.text, '\\textsuperscript{'+run.text+'}')
            elif run.font.subscript:                                                    # Identify subscript text
                t.text = t.text.replace(run.text, '\\textsubscript{'+run.text+'}')
        fulltext.append(t.text)
    return '\n'.join(fulltext)

def latex_text(text):
    # Special cases not recognizable in the Overleaf
    text = text.replace('%','\%')
    text = text.replace('$', '\$')
    text = text.replace('#', '\#')
    text = text.replace('&', '\&')
    text = text.replace(chr(185),'\\textsuperscript{1}')    # ^1
    text = text.replace(chr(178),'\\textsuperscript{2}')    # ^2
    text = text.replace(chr(179),'\\textsuperscript{3}')    # ^3
    text = text.replace('°','\degree ')                     # °
    text = text.replace(chr(153), '\\textsuperscript{TM}')  # ^TM
    text = text.replace('α', '$\\alpha$')
    text = text.replace('β', '$\\beta$')
    text = text.replace('π', '$\\pi$')
    text = text.replace('≈', '$\\approx$')                  # Aprox
    text = text.replace('∑', '$\\Sigma$')                   # Summation
    text = text.replace('≤', '$\\leq$')                     # Greater-Equal
    text = text.replace('≥', '$\\geq$')                     # Less-Equal
    text = text.replace('≠', '$\\neq$')                     # Not-Equal
    return text

arquivo_lista = open('lista_arquivos.txt', 'r')
lista = arquivo_lista.read().split('\n')
diretorio = sys.argv[1] + '\\'                     				# Argumento passado por parâmetro (na invocação do terminal)

for i in lista:
    try:                                                   	# Tenta abrir arquivo como Docx
        resumo=""
        arquivo_escrita=""
        resumo = getText(diretorio+str(i)+'.docx')
        arquivo_escrita = diretorio+'/tex/'+str(i)+'.tex'
        with codecs.open(arquivo_escrita,'w',encoding='utf8') as file:
            file.write(latex_text(resumo))
    except:
        try:
            resumo = open(diretorio+str(i)+'.tex', 'r')         # Caso não consiga tenta abrir como .TEX
            arquivo_escrita = diretorio+'/tex/'+str(i)+'.tex'
            with codecs.open(arquivo_escrita,'w',encoding='utf8') as file:
                file.write((resumo.read()))                     # Arquivo .TEX é lido e copiado para outro diretório
        except:
            continue
