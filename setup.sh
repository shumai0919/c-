# bash "ディレクトリ" で実行

cat << 'EOF' >> ~/.bashrc

# === Universal Language Compiler Shortcuts ===
compile_source() {
    local compiler="$1"
    local src_path="$2"
    local ext="$3"
    local run_cmd="$4"

    if [ -z "$src_path" ]; then
        echo "Usage: $compiler <source file or directory>"
        return 1
    fi

    # パスがファイルかディレクトリかで分岐
    if [ -d "$src_path" ]; then
        src_files="$src_path"/*."$ext"
        output="$src_path/a.out"
        echo "🔨 Building all *.$ext in $src_path ..."
    elif [ -f "$src_path" ]; then
        src_files="$src_path"
        dir_path="$(dirname "$src_path")"
        base_name="$(basename "$src_path" ."$ext")"
        output="$dir_path/$base_name.out"
        echo "🔨 Building $src_path ..."
    else
        echo "❌ File or directory not found: $src_path"
        return 1
    fi

    case "$compiler" in
        cppr)
            g++ -std=c++17 -O2 -Wall $src_files -o "$output" && echo "✅ Built: $output" && "$output"
            ;;
        cr)
            gcc -std=c11 -O2 -Wall $src_files -o "$output" && echo "✅ Built: $output" && "$output"
            ;;
        javar)
            javac $src_files && echo "✅ Compiled Java files" && java -cp "$(dirname "$src_files")" "$(basename "$src_files" .java)"
            ;;
        jsr)
            echo "✅ Running JavaScript: $src_path"
            node "$src_path"
            ;;
        gor)
            echo "✅ Running Go build: $src_path"
            go run "$src_path"
            ;;
        *)
            echo "❌ Unknown compiler: $compiler"
            ;;
    esac
}

cppr()  { compile_source cppr "$1" cpp; }
cr()    { compile_source cr "$1" c; }
javar() { compile_source javar "$1" java; }
jsr()   { compile_source jsr "$1" js; }
gor()   { compile_source gor "$1" go; }

# === End Compiler Shortcuts ===

EOF

# ✅ 追記後すぐに反映
source ~/.bashrc

echo "✅ Setup complete! Commands available: cppr, cr, javar, jsr, gor"