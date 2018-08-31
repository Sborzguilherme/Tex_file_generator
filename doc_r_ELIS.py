import sys
import docx
import codecs
import glob
from unicodedata import normalize

# Função para leitura de texto de um .docx
def getText(filename):
    doc = docx.Document(filename)
    fulltext =[]
    for t in doc.paragraphs:                                                            # All paragraphs
        list_text=[]
        for run in t.runs:
            if run.italic:                                                              # Identify italic text
                if run.text != ' ' and run.text != "." and run.text != "," and run.text != ";":
                    run.text = run.text.replace(run.text, '\\textit{'+run.text+'}')
            elif run.bold:                                                              # Identify Bold text
                if run.text != ' ' and run.text != "." and run.text != "," and run.text != ";":
                    run.text = run.text.replace(run.text, '\\textbf{'+run.text+'}')
            elif run.font.superscript:                                                  # Identify superscript text
                run.text = run.text.replace(run.text, '\\textsuperscript{'+run.text+'}')
            elif run.font.subscript:                                                    # Identify subscript text
                run.text = run.text.replace(run.text, '\\textsubscript{'+run.text+'}')
            list_text.append(run.text)
        t.text = ''.join(list_text)
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
    text = text.replace('μ','$\\mu$')                       # Micro
    text = text.replace('±','$\\pm$')                       # More/Less
    text = text.replace('Ɛ','$\\mathcal{E}$')               # Big epsilon
    text = text.replace('≅','$\\cong$')                     # Approx (2)
    text = text.replace('א','$\\chi$')                      # Chi
    text = text.replace('ω','$\\omega$')                    # omega
    text = text.replace('Ω','$\\Omega$')                    # Omega
    text = text.replace('®','\\textsuperscript{®}')         # Registered mark

    return text

def remover_acentos(txt):
    return normalize('NFKD', txt).encode('ASCII', 'ignore').decode('ASCII')

# String to remove invalid chars from name (First line input in the resume file)
def remove_espaco_ponto_fim_string(palavra):
    verify = True
    while(verify):
        if palavra[len(palavra)-1]  == " " or palavra[len(palavra)-1]  == ".":
            palavra = palavra[:-1]
        else:
            verify = False
    return palavra


diretorio = 'C:/Users/Guilherme/Documents/Univali/CSV_ARQUIVOS/GUI/Arquivos/Arquivos_resumo/' # Deve ser trocado para argumento passado por parâmetro
#diretorio = sys.argv[1] + '\\'

files = glob.glob(diretorio+'*.docx')   # Tenta abrir todos os arquivos docx
for name in files:                      # Percorre todos os arquivos docx na pasta informada
    try:
        with open(name) as f:
            resumo=""                   # Limpa as strings
            nome_arq_escrita=""
            resumo = getText(name)                                              # Pega o conteúdo do arquivo
            lista = resumo.split('\n')                                          # Separa as linhas em uma lista
            nome_arq_escrita = remover_acentos(lista[0])
            nome_arq_escrita = nome_arq_escrita.lower()
            nome_arq_escrita = remove_espaco_ponto_fim_string(nome_arq_escrita) # Remove spaces and end periods
            nome_arq_escrita = diretorio+'/tex/'+nome_arq_escrita+ '.tex'       # Primeira posição da lista é sempre o nome do aluno
            if lista[1] == "" or lista[2] == " ":                               # Verificar em qual linha está o resumo
                resumo = lista[2]
            else:
                resumo = lista[1]
            with codecs.open(nome_arq_escrita,'w',encoding='utf8') as arquivo_escrita:
                arquivo_escrita.write(latex_text(resumo))               # Gera um arquivo .TEX contendo o resumo do trabalho
            f.close()
    except:
        continue

files_tex = glob.glob(diretorio+'*.tex')        # Tenta abrir todos os arquivos docx
for name_tex in files_tex:                      # Percorre todos os arquivos docx na pasta informada
    try:
        with codecs.open(name_tex,'rb',encoding='utf8') as f_tex:
            resumo=""                                                           # Limpa as strings
            nome_arq_escrita=""
            resumo = f_tex.read()                                               # Pega o conteúdo do arquivo
            lista = resumo.split('\n')                                          # Separa as linhas em uma lista
            nome_arq_escrita = remover_acentos(lista[0])
            nome_arq_escrita = nome_arq_escrita.lower()
            nome_arq_escrita = nome_arq_escrita[0:len(nome_arq_escrita)-2]      # Throw away last two positions
            nome_arq_escrita = diretorio+'/tex/'+nome_arq_escrita+ '.tex'       # Primeira posição da lista é sempre o nome do aluno
            if lista[1] == "":                                                  # Verificar em qual linha está o resumo
                resumo = lista[2]
            else:
                resumo = lista[1]
            with codecs.open(nome_arq_escrita,'w',encoding='utf8') as arquivo_escrita:
                arquivo_escrita.write(latex_text(resumo))               # Gera um arquivo .TEX contendo o resumo do trabalho
    except:
        continue
